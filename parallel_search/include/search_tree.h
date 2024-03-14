#pragma once
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <list>

#include "utils.h"


class SearchTreeNode {
    private:
        std::shared_mutex mutex;
        const size_t depth;
        std::optional<std::string> item;
        std::optional<std::unordered_map<char, std::shared_ptr<SearchTreeNode>>> characterMap;

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

        void getAllItems(std::list<std::string>* results, size_t nThreads);
        std::optional<std::vector<std::string>> find(const std::string &s, size_t nThreads);
};

class SearchTree {
    private:
        SearchTreeNode root;

    public:
        SearchTree();
        void addString(const std::string &element);
        void addElements(const std::vector<std::string> &elements, size_t nThreads);

        std::vector<std::string> find(const std::string &s, size_t nThreads);
};
