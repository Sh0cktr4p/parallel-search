#include "search_tree.h"

#ifndef SKIP_DIS_SHIT

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

void SearchTreeNode::getAllItems(std::vector<std::string>* results) {
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

    if (children.size() > 0) {
        std::vector<std::thread> threadPool;
        std::vector<std::vector<std::string>> childResults(children.size());

        for (size_t i = 1; i < children.size(); i++) {
            // childResults.emplace_back();
            children[i]->getAllItems(&childResults[i]);
            
            threadPool.emplace_back(
                &SearchTreeNode::getAllItems,
                children[i],
                &childResults[i]
            );
        }

       // childResults.emplace_back();
        children[0]->getAllItems(&childResults[0]);

        // Perform work on main thread

        for (std::thread& thread : threadPool) {
            thread.join();
        }

        size_t nTotalChildResults{0};
        for (std::vector<std::string> &res : childResults) {
            nTotalChildResults += res.size();
        }

        results->reserve(results->size() + nTotalChildResults);

        for (std::vector<std::string> &res : childResults) {
            results->insert(results->end(), res.begin(), res.end());
        }
    }
}

std::optional<std::vector<std::string>> SearchTreeNode::find(const std::string &s) {
    SearchTreeNode* subtree = this->getMatchingSubtree(s);

    if (subtree == nullptr) {
        return std::nullopt;
    }
    else {
        std::vector<std::string> results;
        subtree->getAllItems(&results);
        return results;
    }
}



SearchTree::SearchTree(): root(0) {}

void SearchTree::addString(const std::string &element) {
    this->root.addString(element);
}

void SearchTree::addElements(const std::vector<std::string> &elements) {
    size_t maxNThreads{10};

    const size_t realNThreads = std::min(maxNThreads, elements.size());

    std::vector<std::thread> threadPool;

    auto threadFn = [this, &elements] (size_t startDiff, size_t endDiff) {
        for (size_t i = startDiff; i < endDiff; i++) {
            this->addString(elements[i]);
        }
    };

    for (size_t i = 0; i < realNThreads; i++) {
        size_t startDiff = std::round(i * elements.size() * 1.0 / realNThreads);
        size_t endDiff = std::round((i + 1) * elements.size() * 1.0 / realNThreads);
        
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

std::vector<std::string> SearchTree::find(const std::string &s) {
    return this->root.find(s).value_or(std::vector<std::string>{});
}
#endif