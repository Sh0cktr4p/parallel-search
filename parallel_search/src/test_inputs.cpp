#include "test_inputs.h"
#include <random>
#include <algorithm>

int pow(int b, unsigned int e) {
    int v = 1;
    for (unsigned int i = 0; i < e; i++) {
        v *= b;
    }

    return v;
}

std::string getStringFromIndex(unsigned int index, std::vector<unsigned int> &powers) {
    std::string s{};
    s.reserve(powers.size());
    for (unsigned int power : powers) {
        unsigned int char_index = (index / power) % 26;
        char c = 'A' + char_index;
        s = c + s;
    }

    return s;
}

std::string getRandomString(unsigned int length) {
    std::string s(length, 0);

    std::generate(s.begin(), s.end(), []() -> char {
        return 'A' + (rand() % 26);
    });

    return s;
}


std::vector<std::string> getFixedSizeTestInput(unsigned int stringLength) {
    std::vector<std::string> vec;
    unsigned int vector_length = pow(26, stringLength);
    vec.reserve(vector_length);
    std::vector<unsigned int> powers(stringLength);
    for (unsigned int i = 0; i < powers.size(); i++) {
        powers[i] = pow(26, i);
    }
    
    for (unsigned int i = 0; i < vector_length; i++) {
        vec.push_back(getStringFromIndex(i, powers));
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);

    return vec;
}

std::vector<std::string> getRandomSizeTestInput(
    unsigned int numElements,
    unsigned int minLength,
    unsigned int maxLength
) {
    std::vector<std::string> vec;
    vec.reserve(numElements);

    for (unsigned int i = 0; i < numElements; i++) {
        vec.push_back(getRandomString(minLength + (rand() % (maxLength - minLength))));
    }

    return vec;
}