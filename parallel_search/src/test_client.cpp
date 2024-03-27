#include "test_client.h"

enum AlgorithmSelection {
    LINEAR_SEARCH,
    TREE_SEARCH,
    BOTH,
};

template <typename S>
std::ostream& operator<<(
    std::ostream& os,
    const std::vector<S>& vector
) {
    os << "[ ";
    for (auto element : vector) {
        os << element << " ";
    }
    os << "]";
    return os;
}

template <typename S>
std::ostream& operator<<(
    std::ostream& os,
    const std::set<S>& set
) {
    os << "{ ";
    for (auto element : set) {
        os << element << " ";
    }
    os << "}";
    return os;
}

void profile(SearchAlgorithm &a, std::vector<std::string> queries) {
    auto startProcessData = std::chrono::system_clock::now();
    a.processData();
    auto startSearch = std::chrono::system_clock::now();
    for (std::string &s : queries) {
        a.search(s);
    }
    auto endSearch = std::chrono::system_clock::now();

    std::chrono::duration<double> processDataDuration = startSearch - startProcessData;
    std::chrono::duration<double> searchDuration = endSearch - startSearch;
    std::chrono::duration<double> totalDuration = endSearch - startProcessData;

    std::cout << "Time for processing data:\t\t" << processDataDuration.count() << "s" << std::endl;
    std::cout << "Time for searching " << queries.size() << " elements:\t\t" << searchDuration.count() << "s" << std::endl;
    std::cout << "Total time:\t\t\t\t" << totalDuration.count() << "s" << std::endl;
}

void profileOnNRandomQueries(SearchAlgorithm &a, size_t nQueries) {
    std::vector<std::string> queries = getRandomLengthExampleInput(nQueries, 0, 4);
    profile(a, queries);
}

void compareBothAlgorithms(size_t nThreads, size_t nQueries, std::vector<std::string> inputData) {
    std::vector<std::string> queries = getRandomLengthExampleInput(nQueries, 0, 4);
    ParallelLinearSearchAlgorithm lin(inputData, nThreads);
    TreeSearchAlgorithm tr(inputData, nThreads);
    std::cout << "Linear Search:" << std::endl;
    profile(lin, queries);
    std::cout << "Tree Search:" << std::endl;
    profile(tr, queries);

    std::cout << "Comparing Outputs:" << std::endl;
    std::vector<std::string> compQueries{"A", "AB", "ABC", "ABBA"};
    for (auto& query : compQueries) {
        auto linRes = lin.search(query);
        auto trRes = tr.search(query);
        //std::cout << "Linear Search:" << lin.search(query) << std::endl;
        //std::cout << "Tree Search:" << tr.search(query) << std::endl;
        std::set<std::string> linSet{linRes.begin(), linRes.end()};
        std::set<std::string> trSet{trRes.begin(), trRes.end()};
        std::set<std::string> diff;
        std::set_difference(linSet.begin(), linSet.end(), trSet.begin(), trSet.end(), std::inserter(diff, diff.begin()));
        std::cout << "Set difference for " << query << ": " << diff << std::endl;
    }
}

void runTestClient() {
    while (true) {
        std::cout << "Welcome to the sorting algorithm test client! Choose your action:" << std::endl;
        std::cout << "\t[l/L]: test a parallelized linear search algorithm" << std::endl;
        std::cout << "\t[t/T]: test a parallelized tree search algorithm" << std::endl;
        std::cout << "\t[c/C]: compare both algorithms on the same input data" << std::endl;
        std::cout << "\t[q/Q]: quit the test client" << std::endl;
        std::cout << "\tl/t/q" << std::endl;
        std::string action;
        std::cin >> action;

        AlgorithmSelection a;

        if (action == "q" || action == "Q") {
            std::cout << "Quitting Test Client." << std::endl;
            return;
        }
        else if (action == "l" || action == "L") {
            std::cout << "Parallelized linear search chosen." << std::endl;
            a = AlgorithmSelection::LINEAR_SEARCH;
        }
        else if (action == "t" || action == "T") {
            std::cout << "Parallelized tree search chosen." << std::endl;
            a = AlgorithmSelection::TREE_SEARCH;
        }
        else if (action == "c" || action == "C") {
            std::cout << "Comparing both algorithms." << std::endl;
            a = AlgorithmSelection::BOTH;
        }
        else {
            std::cout << "Unknown action." << std::endl;
            continue;
        }

        size_t nThreads{1};
        std::cout << "Enter the number of threads" << std::endl;
        std::cin >> nThreads;
        
        size_t nQueries{1};
        std::cout << "Enter the number of queries" << std::endl;
        std::cin >> nQueries;

        std::vector<std::string> inputData{getFixedSizeExampleInput(4)};

        if (a == AlgorithmSelection::LINEAR_SEARCH) {
            ParallelLinearSearchAlgorithm algo(inputData, nThreads);
            profileOnNRandomQueries(algo, nQueries);
        }
        else if (a == AlgorithmSelection::TREE_SEARCH) {
            TreeSearchAlgorithm algo(inputData, nThreads);
            profileOnNRandomQueries(algo, nQueries);
        }
        else if (a == AlgorithmSelection::BOTH) {
            compareBothAlgorithms(nThreads, nQueries, inputData);
        }
    }
}

void useTreeSearch() {
    auto data = getFixedSizeExampleInput(4);
    auto tr = TreeSearchAlgorithm(data, 1);
    auto startTreeGen = std::chrono::system_clock::now();
    tr.processData();
    auto endTreeGen = std::chrono::system_clock::now();

    std::chrono::duration<double> treeGenDuration = endTreeGen - startTreeGen;
    std::cout << "Building the tree took " << treeGenDuration.count() << "s" << std::endl;

    while (true) {
        std::cout << "Please input an all-capital string" << std::endl;
        std::string input;
        std::cin >> input;

        if(input == "<empty>") {
            input = "";
        }
        auto startSearch = std::chrono::system_clock::now();
        auto matches = tr.search(input);
        auto endSearch = std::chrono::system_clock::now();
        std::chrono::duration<double> searchDuration = endSearch - startSearch;
        std::cout << matches << std::endl;
        std::cout << "Searching took " << searchDuration.count() << "s" << std::endl;
    }
}
