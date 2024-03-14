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
        SearchTreeNode* child = this->getChild(this->getKeyChar(s));
        if (child != nullptr) {
            return child->getMatchingSubtree(s);
        }
        else {
            return nullptr;
        }
    }
}

void SearchTreeNode::addLeaf(const std::string& s) {
    this->characterMap.value().emplace(this->getKeyChar(s), std::make_unique<SearchTreeNode>(this->depth + 1, s));
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
    if (this->item == s) {
        // Do not expand if the element is already in the tree
        return;
    }

    if (s.length() == this->depth) {
        // Transform the node to an inner node and replace the item
        // If already an inner node, toInnerNode does nothing
        this->toInnerNode();
        this->item = s;
    }
    else {
        if (this->isLeaf()) {
            this->toInnerNode();
        }
        SearchTreeNode* childNode = this->getChild(this->getKeyChar(s));
        if (childNode == nullptr) {
            // Child was not found (it is possible that the node is a leaf)
            this->addLeaf(s);
        }
        else {
            return childNode->addString(s);
        }
    }
}

std::vector<std::string> SearchTreeNode::getAllItems() {
    std::vector<std::string> result;
    if (this->item.has_value()) {
        result.push_back(this->item.value());
    }
    if (this->characterMap.has_value()) {
        for (auto &[key, childNode] : this->characterMap.value()) {
            std::vector<std::string> childItems = childNode->getAllItems();
            result.reserve(result.size() + childItems.size());
            result.insert(result.end(), childItems.begin(), childItems.end());
        }
    }

    return result;
}

std::optional<std::vector<std::string>> SearchTreeNode::find(const std::string &s) {
    SearchTreeNode* subtree = this->getMatchingSubtree(s);

    if (subtree == nullptr) {
        return std::nullopt;
    }
    else {
        return subtree->getAllItems();
    }
}




SearchTree::SearchTree(): root(0) {}

void SearchTree::addString(const std::string &element) {
    this->root.addString(element);
}

void SearchTree::addElements(const std::vector<std::string> &elements) {
    for (const std::string &element : elements) {
        this->addString(element);
    }
}

std::vector<std::string> SearchTree::find(const std::string &s) {
    return this->root.find(s).value_or(std::vector<std::string>{});
}
#endif