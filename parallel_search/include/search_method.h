#pragma once

#include <vector>
#include <string>

#include "search_tree.h"


class SearchMethod {
    protected:
        std::vector<std::string>& data;

    public:
        SearchMethod(std::vector<std::string>& data);

        virtual std::vector<std::string> search(std::string &keyword) = 0;
};


class LinearSearchMethod: SearchMethod {
    public:
        LinearSearchMethod(std::vector<std::string>& data);

        std::vector<std::string> search(std::string &keyword);
};


class TreeSearchMethod: SearchMethod {
    private:
        SearchTree tree;
    
    public:
        TreeSearchMethod(std::vector<std::string>& data);

        std::vector<std::string> search(std::string &keyword);
};
