#pragma once

#include <vector>
#include <string>

std::vector<std::string> getFixedSizeTestInput(
    unsigned int length
);

std::vector<std::string> getRandomSizeTestInput(
    unsigned int numElements,
    unsigned int minLength,
    unsigned int maxLength
);
