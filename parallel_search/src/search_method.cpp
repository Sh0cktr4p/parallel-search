#include "search_method.h"


SearchMethod::SearchMethod(const std::vector<std::string>& data): data{data} {}

LinearSearchMethod::LinearSearchMethod(const std::vector<std::string>& data): SearchMethod{data} {}

std::vector<std::string> LinearSearchMethod::search(const std::string &keyword) {
    std::vector<std::string> results;

    for (const std::string &element : this->data) {
        if(element.rfind(keyword, 0) == 0) {
            results.push_back(element);
        }
    }

    return results;
}

ParallelLinearSearchMethod::ParallelLinearSearchMethod(const std::vector<std::string>& data, size_t nThreads): SearchMethod(data), nThreads{nThreads} {}


void ParallelLinearSearchMethod::searchRange(
    std::vector<std::string>* results,
    const std::string& keyword,
    size_t begin,
    size_t end
) {
    for (size_t i = begin; i < end; i++) {
        if (data[i].length() >= keyword.length() && data[i].rfind(keyword, 0) == 0) {
            results->push_back(data[i]);
        }
    }
}

std::vector<std::string> ParallelLinearSearchMethod::search(const std::string &keyword) {
    // Do not create more threads than elements in the vector
    const size_t realNThreads = std::min(this->nThreads, this->data.size());

    std::vector<std::thread> threadPool;
    std::vector<std::vector<std::string>> results(realNThreads);

    for (size_t i = 0; i < realNThreads; i++) {
        size_t startIndex = getStartIndex(this->data.size(), realNThreads, i);
        size_t endIndex = getEndIndex(this->data.size(), realNThreads, i);

        if (i < realNThreads - 1) {
            threadPool.emplace_back(&ParallelLinearSearchMethod::searchRange, this, &results[i], keyword, startIndex, endIndex);
        }
        else {
            // Also perform work on the main thread
            this->searchRange(&results[i], keyword, startIndex, endIndex);
        }
    }

    for (std::thread& thread : threadPool) {
        thread.join();
    }

    std::vector<std::string> flattenedResults;
    for (std::vector<std::string> &v : results) {
        flattenedResults.insert(flattenedResults.end(), v.begin(), v.end());
    }

    return flattenedResults;
}

TreeSearchMethod::TreeSearchMethod(const std::vector<std::string>& data, size_t nThreads): SearchMethod(data), tree(), nThreads{nThreads} {
    this->tree.addElements(this->data, this->nThreads);
}

std::vector<std::string> TreeSearchMethod::search(const std::string &keyword) {
    return this->tree.find(keyword, this->nThreads);
}