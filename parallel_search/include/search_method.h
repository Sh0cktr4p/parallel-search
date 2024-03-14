#pragma once

#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <future>

#include "search_tree.h"


class SearchMethod {
    protected:
        const std::vector<std::string>& data;

    public:
        SearchMethod(const std::vector<std::string>& data);

        virtual std::vector<std::string> search(const std::string &keyword) = 0;
};


class LinearSearchMethod: public SearchMethod {
    public:
        LinearSearchMethod(const std::vector<std::string>& data);

        std::vector<std::string> search(const std::string &keyword) override;
};


class ParallelLinearSearchMethod: public SearchMethod {
    private:
        size_t nThreads;

        void searchRange(std::vector<std::string>* results, const std::string &keyword, size_t begin, size_t end);
    public:
        ParallelLinearSearchMethod(const std::vector<std::string>& data, size_t nThreads);

        std::vector<std::string> search(const std::string &keyword) override;
};


#ifndef SKIP_DIS_SHIT
class TreeSearchMethod: public SearchMethod {
    private:
        SearchTree tree;
        size_t nThreads;
    
    public:
        TreeSearchMethod(const std::vector<std::string>& data, size_t nThreads);

        std::vector<std::string> search(const std::string &keyword) override;
};
#endif