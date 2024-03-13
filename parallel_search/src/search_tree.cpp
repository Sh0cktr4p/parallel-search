#include "search_tree.h"

#ifndef SKIP_DIS_SHIT
SearchTreeNode::SearchTreeNode(size_t depth): depth{depth}, item{}, character_map{} {}

SearchTreeNode::SearchTreeNode(size_t depth, const std::string &item): depth{depth}, item{item}, character_map{} {}

bool SearchTreeNode::isLeaf() {
    // Inner node may also have items (if depth == length)
    return !this->character_map.has_value();
}

SearchTreeNode *SearchTreeNode::getChild(const std::string &s) {
    if (s.length() <= this->depth || !this->character_map.has_value()) {
        return nullptr;
    }
    else {
        auto search = this->character_map.value().find(s[this->depth]);
        if (search == this->character_map.value().end()) {
            return nullptr;
        }
        else {
            return search->second.get();
        }
    }
}

SearchTreeNode* SearchTreeNode::getMatchingSubtree(const std::string &s) {
    if (s.length() == this->depth) {
        return this;
    }
    else {
        SearchTreeNode* child = this->getChild(s);
        if (child != nullptr) {
            return child->getMatchingSubtree(s);
        }
        else {
            return nullptr;
        }
    }
}

void SearchTreeNode::addString(const std::string &s) {
    if (this->item.has_value() && this->item.value() == s) {
        // Do nothing when storing an already stored string
        return;
    }

    if (this->isLeaf()) {
        // Initialize the character map, store current item (transform from leaf to inner node)
        this->character_map = std::unordered_map<char, std::unique_ptr<SearchTreeNode>>();

        if (this->item.has_value() && this->item.value().length() > this->depth) {
            this->character_map.value()[this->item.value()[this->depth]] = std::make_unique<SearchTreeNode>(this->depth + 1, this->item.value());
            this->item.reset();
        }
    }

    if (s.length() == this->depth) {
        this->item = s;
    }
    else {
        SearchTreeNode* child_node = this->getChild(s);
        if (child_node == nullptr) {
            this->character_map.value()[s[this->depth]] = std::make_unique<SearchTreeNode>(this->depth + 1, s);
        }
        else {
            child_node->addString(s);
        }
    }
}

std::vector<std::string> SearchTreeNode::getAllItems() {
    std::vector<std::string> result;
    if (this->item.has_value()) {
        result.push_back(this->item.value());
    }
    if (this->character_map.has_value()) {
        for (auto &[key, childNode] : this->character_map.value()) {
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