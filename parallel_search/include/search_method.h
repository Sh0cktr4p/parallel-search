#pragma once

#include <vector>
#include <string>

#include "search_tree.h"


class SearchMethod {
    protected:
        const std::vector<std::string>& data;

    public:
        SearchMethod(const std::vector<std::string>& data);

        virtual std::vector<std::string> search(const std::string &keyword) = 0;
};


class LinearSearchMethod: SearchMethod {
    public:
        LinearSearchMethod(const std::vector<std::string>& data);

        std::vector<std::string> search(const std::string &keyword) override;
};


class TreeSearchMethod: SearchMethod {
    private:
        SearchTree tree;
    
    public:
        TreeSearchMethod(const std::vector<std::string>& data);

        std::vector<std::string> search(const std::string &keyword) override;
};
