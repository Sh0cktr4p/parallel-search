#pragma once
#define SKIP_DIwS_SHIT 1
#ifndef SKIP_DIS_SHIT
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>


class SearchTreeNode {
    private:
        std::shared_mutex mutex;
        const size_t depth;
        std::optional<std::string> item;
        std::optional<std::unordered_map<char, std::unique_ptr<SearchTreeNode>>> characterMap;

        char getKeyChar(const std::string &s);
        SearchTreeNode* getChild(char c);
        SearchTreeNode* getMatchingSubtree(const std::string& s);
        void addLeaf(const std::string& s);
        void toInnerNode();
    public:
        SearchTreeNode(size_t depth);
        SearchTreeNode(size_t depth, const std::string& item);

        bool isLeaf();
        // If a subnode already exists for the given character, return it
        // Otherwise, create a new node
        void addString(const std::string &s);

        void getAllItems(std::vector<std::string>* results);
        std::optional<std::vector<std::string>> find(const std::string &s);
};

class SearchTree {
    private:
        SearchTreeNode root;
    
    public:
        SearchTree();
        void addString(const std::string &element);
        void addElements(const std::vector<std::string> &elements);

        std::vector<std::string> find(const std::string &s);
};
#endif