#include "search_method.h"


SearchAlgorithm::SearchAlgorithm(const std::vector<std::string>& data): data{data} {}

ParallelLinearSearchAlgorithm::ParallelLinearSearchAlgorithm(const std::vector<std::string>& data, size_t nThreads): SearchAlgorithm(data), nThreads{nThreads} {}

void ParallelLinearSearchAlgorithm::processData() {}

std::vector<std::string> ParallelLinearSearchAlgorithm::search(const std::string &keyword) {
    // Do not create more threads than elements in the vector
    const size_t realNThreads = std::min(this->nThreads, this->data.size());

    std::vector<std::thread> threadPool;
    std::vector<std::vector<std::string>> results(realNThreads);

    auto threadFn = [this, keyword](std::vector<std::string>* resultsOfThread, size_t beginIndex, size_t endIndex) {
        for (size_t i = beginIndex; i < endIndex; i++) {
            if (matchPrefix(this->data[i], keyword)) {
                resultsOfThread->push_back(data[i]);
            }
        }
    };

    for (size_t i = 0; i < realNThreads; i++) {
        size_t startIndex = getStartIndex(this->data.size(), realNThreads, i);
        size_t endIndex = getEndIndex(this->data.size(), realNThreads, i);

        if (i < realNThreads - 1) {
            threadPool.emplace_back(
                threadFn,
                &results[i],
                startIndex,
                endIndex
            );
        }
        else {
            // Also perform work on the main thread
            threadFn(
                &results[i],
                startIndex,
                endIndex
            );
        }
    }

    for (std::thread& thread : threadPool) {
        thread.join();
    }

    std::vector<std::string> flattenedResults;
    insertVectors(flattenedResults, results);

    return flattenedResults;
}

TreeSearchAlgorithm::TreeSearchAlgorithm(const std::vector<std::string>& data, size_t nThreads): SearchAlgorithm(data), tree(), nThreads{nThreads} {}

void TreeSearchAlgorithm::processData() {
    this->tree.addElements(this->data, this->nThreads);
}

std::vector<std::string> TreeSearchAlgorithm::search(const std::string &keyword) {
    return this->tree.find(keyword, this->nThreads);
}