#include "search_tree.h"

char SearchTreeNode::getKeyChar(const std::string &s) {
    return s[this->depth];
}

SearchTreeNode* SearchTreeNode::getChild(char c) {
    if (this->isLeaf()) {
        return nullptr;
    }

    auto search = this->characterMap.value().find(c);
    if (search == this->characterMap.value().end()) {
        return nullptr;
    }
    else {
        return search->second.get();
    }
}

SearchTreeNode* SearchTreeNode::getMatchingSubtree(const std::string &s) {
    if (s.length() == this->depth) {
        return this;
    }
    else {
        SearchTreeNode* child;
        {
            std::shared_lock lock(this->mutex);
            child = this->getChild(this->getKeyChar(s));
        }
        if (child != nullptr) {
            return child->getMatchingSubtree(s);
        }
        else {
            return nullptr;
        }
    }
}

void SearchTreeNode::addLeaf(const std::string& s) {
    this->characterMap.value().emplace(this->getKeyChar(s), std::make_shared<SearchTreeNode>(this->depth + 1, s));
}

void SearchTreeNode::toInnerNode() {
    if (!this->isLeaf()) {
        return;
    }

    this->characterMap.emplace();

    if (this->item.has_value() && this->item.value().length() > this->depth) {
        this->addLeaf(this->item.value());
        this->item.reset();
    }
}



SearchTreeNode::SearchTreeNode(size_t depth): depth{depth} {}

SearchTreeNode::SearchTreeNode(size_t depth, const std::string &item): depth{depth}, item{item} {}


bool SearchTreeNode::isLeaf() {
    // Inner node may also have items (if depth == length)
    return !this->characterMap.has_value();
}

void SearchTreeNode::addString(const std::string &s) {
    SearchTreeNode* childNode = nullptr;
    {
        std::shared_lock lock(this->mutex);
        if (s.length() > this->depth && !this->isLeaf()) {
            childNode = this->getChild(this->getKeyChar(s));
        }
    }

    if (childNode == nullptr) {
        // Node has to be modified
        std::unique_lock lock(this->mutex);
        if (this->item == s) {
            return;
        }
        if (s.length() == this->depth) {
            this->toInnerNode();
            this->item = s;
            return;
        }
        else {
            if(this->isLeaf()) {
                this->toInnerNode();
            }
            childNode = this->getChild(this->getKeyChar(s));
            if (childNode == nullptr) {
                this->addLeaf(s);
                return;
            }
        }
    }

    childNode->addString(s);
}

void SearchTreeNode::getAllItems(std::list<std::string>* results, size_t nThreads) {
    std::vector<SearchTreeNode*> children;
    {
        std::shared_lock lock(this->mutex);
        if (this->characterMap.has_value()) {
            for (auto &[key, childNode] : this->characterMap.value()){
                children.push_back(childNode.get());
            }
        }
        if (this->item.has_value()) {
            results->push_back(this->item.value());
        }
    }

    // How many threads to run at this level and how many to create at each child
    const std::vector<size_t> childNThreads = getDistributedThreadCounts(nThreads, children.size());

    if (children.size() > 0) {
        std::vector<std::thread> threadPool;
        std::list<std::list<std::string>> resultsPerThread(childNThreads.size());

        auto fn = [&children](std::list<std::string>* resultsOfMyThread, size_t startChildIdx, size_t endChildIdx, size_t nChildThreads) {
            for (size_t childIndex = startChildIdx; childIndex < endChildIdx; childIndex++) {
                std::list<std::string> resultsOfChild;
                children[childIndex]->getAllItems(
                    &resultsOfChild,
                    nChildThreads
                );

                resultsOfMyThread->splice(resultsOfMyThread->end(), resultsOfChild);
            }
        };

        size_t threadIndex{0};
        for (std::list<std::string> &resultsOfThread : resultsPerThread) {
            size_t startChildIdx = getStartIndex(children.size(), childNThreads.size(), threadIndex);
            size_t endChildIdx = getEndIndex(children.size(), childNThreads.size(), threadIndex);

            if (threadIndex < childNThreads.size() - 1 && false) {
                threadPool.emplace_back(
                    fn,
                    &resultsOfThread,
                    startChildIdx,
                    endChildIdx,
                    childNThreads[threadIndex]
                );
            }
            else {
                // Perform work on main thread
                fn(
                    &resultsOfThread,
                    startChildIdx,
                    endChildIdx,
                    childNThreads[threadIndex]
                );
            }
        }

        for (std::thread& thread : threadPool) {
            thread.join();
        }

        for (std::list<std::string> &resultOfThread : resultsPerThread) {
            results->splice(results->end(), resultOfThread);
        }
    }
}

std::optional<std::vector<std::string>> SearchTreeNode::find(const std::string &s, size_t nThreads) {
    SearchTreeNode* subtree = this->getMatchingSubtree(s);

    if (subtree == nullptr) {
        return std::nullopt;
    }
    else {
        std::list<std::string> results;
        subtree->getAllItems(&results, nThreads);

        return std::vector<std::string>{results.begin(), results.end()};
    }
}



SearchTree::SearchTree(): root(0) {}

void SearchTree::addString(const std::string &element) {
    this->root.addString(element);
}

void SearchTree::addElements(const std::vector<std::string> &elements, size_t nThreads) {
    const size_t realNThreads = std::min(nThreads, elements.size());

    std::vector<std::thread> threadPool;

    auto threadFn = [this, &elements] (size_t startDiff, size_t endDiff) {
        for (size_t i = startDiff; i < endDiff; i++) {
            this->addString(elements[i]);
        }
    };

    for (size_t i = 0; i < realNThreads; i++) {
        size_t startDiff = getStartIndex(elements.size(), realNThreads, i);
        size_t endDiff = getEndIndex(elements.size(), realNThreads, i);
        
        if (i < realNThreads - 1) {
            threadPool.emplace_back(threadFn, startDiff, endDiff);
        }
        else {
            threadFn(startDiff, endDiff);
        }
    }

    for (std::thread& thread : threadPool) {
        thread.join();
    }
}

std::vector<std::string> SearchTree::find(const std::string &s, size_t nThreads) {
    return this->root.find(s, nThreads).value_or(std::vector<std::string>{});
}