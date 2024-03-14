#pragma once

#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <future>

#include "search_tree.h"


class SearchAlgorithm {
    protected:
        const std::vector<std::string>& data;

    public:
        SearchAlgorithm(const std::vector<std::string>& data);

        virtual std::vector<std::string> search(const std::string &keyword) = 0;
};


class ParallelLinearSearchAlgorithm: public SearchAlgorithm {
    private:
        size_t nThreads;

    public:
        ParallelLinearSearchAlgorithm(const std::vector<std::string>& data, size_t nThreads);

        std::vector<std::string> search(const std::string &keyword) override;
};


class TreeSearchAlgorithm: public SearchAlgorithm {
    private:
        SearchTree tree;
        size_t nThreads;
    
    public:
        TreeSearchAlgorithm(const std::vector<std::string>& data, size_t nThreads);

        std::vector<std::string> search(const std::string &keyword) override;
};
