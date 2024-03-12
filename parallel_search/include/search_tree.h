#pragma once
#define SKIPe_DIS_SHIT 1
#ifndef SKIP_DIS_SHIT
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <memory>


class SearchTreeNode {
    private:
        size_t depth;
        std::optional<std::string> item;
        std::optional<std::unordered_map<char, std::unique_ptr<SearchTreeNode>>> character_map;

        SearchTreeNode* getChild(std::string& s);
        SearchTreeNode* getMatchingSubtree(std::string& s);
    public:
        SearchTreeNode(size_t depth);
        SearchTreeNode(size_t depth, std::string& item);

        bool isLeaf();
        // If a subnode already exists for the given character, return it
        // Otherwise, create a new node
        void addString(std::string &s);

        std::vector<std::string> getAllItems();
        std::optional<std::vector<std::string>> find(std::string &s);
};

class SearchTree {
    private:
        SearchTreeNode root;
    
    public:
        SearchTree();
        void addString(std::string &element);
        void addElements(std::vector<std::string> &elements);

        std::vector<std::string> find(std::string &s);
};
#endif