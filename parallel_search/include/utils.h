#pragma once

#include <vector>
#include <cmath>
#include <string>


std::vector<size_t> getDistributedThreadCounts(size_t maxNThreads, size_t nPossibleDirectChildren);

size_t getStartIndex(size_t nElements, size_t nGroups, size_t groupIndex);

size_t getEndIndex(size_t nElements, size_t nGroups, size_t groupIndex);

void insertVectors(std::vector<std::string>& vector, const std::vector<std::vector<std::string>>& additionalVectors);

bool matchPrefix(const std::string &s, const std::string &query);