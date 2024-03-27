#include "utils.h"

std::vector<size_t> getDistributedThreadCounts(size_t maxNThreads, size_t nPossibleDirectChildren) {
    size_t myNThreads = std::min(maxNThreads, nPossibleDirectChildren);     // Number of threads on this level
    std::vector<size_t> childNThreads(myNThreads, 1);                       // Minimum of one thread for each child
    if (myNThreads < maxNThreads) {
        size_t remainingThreads{maxNThreads - myNThreads};
        for (size_t i = 0; i < childNThreads.size(); i++) {
            // distribute remaining threads
            childNThreads[i] += remainingThreads / childNThreads.size();
            if (i < remainingThreads % childNThreads.size()) {
                childNThreads[i]++;
            }
        }
    }

    return childNThreads;
}

size_t getStartIndex(size_t nElements, size_t nGroups, size_t groupIndex) {
    return std::round(groupIndex * nElements * 1.0 / nGroups);
}

size_t getEndIndex(size_t nElements, size_t nGroups, size_t groupIndex) {
    return std::round((groupIndex + 1) * nElements * 1.0 / nGroups);
}

void insertVectors(std::vector<std::string>& vector, const std::vector<std::vector<std::string>>& additionalVectors) {
    size_t nAdditionalElements{0};
    for (const std::vector<std::string>& v : additionalVectors) {
        nAdditionalElements += v.size();
    }

    vector.reserve(vector.size() + nAdditionalElements);
    for (const std::vector<std::string>& v : additionalVectors) {
        vector.insert(vector.end(), v.begin(), v.end());
    }
}

bool matchPrefix(const std::string &s, const std::string &queryStr) {
    return s.length() >= queryStr.length() && s.rfind(queryStr, 0) == 0;
}
