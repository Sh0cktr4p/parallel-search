#pragma once

#include <vector>
#include <string>

int pow(int base, unsigned int exponent);

std::string getRandomString(unsigned int length, const std::string& charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

// Vector of all permutations of strings with length <length>
std::vector<std::string> getFixedSizeExampleInput(
    unsigned int length,
    std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
);

// Vector of random strings with length between <minLength> and <maxLength>
std::vector<std::string> getRandomLengthExampleInput(
    unsigned int numElements,
    unsigned int minLength,
    unsigned int maxLength,
    std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
);
