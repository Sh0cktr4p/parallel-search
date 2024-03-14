#include "example_inputs.h"
#include <random>
#include <algorithm>

int pow(int b, unsigned int e) {
    int v = 1;
    for (unsigned int i = 0; i < e; i++) {
        v *= b;
    }

    return v;
}

std::string getStringFromIndex(
    unsigned int index,
    const std::vector<unsigned int> &powers,
    const std::string& charSet
) {
    std::string s{};
    s.reserve(powers.size());
    for (unsigned int power : powers) {
        unsigned int charIndex = (index / power) % charSet.length();
        char c = charSet[charIndex];
        s = c + s;
    }

    return s;
}

std::string getRandomString(
    unsigned int length,
    const std::string& charSet
) {
    std::string s(length, 0);

    std::generate(s.begin(), s.end(), [&charSet]() -> char {
        return charSet[rand() % charSet.length()];
    });

    return s;
}

std::vector<std::string> getFixedSizeExampleInput(
    unsigned int stringLength,
    std::string charSet
) {
    std::vector<std::string> vec;
    unsigned int vector_length = pow(charSet.length(), stringLength);
    vec.reserve(vector_length);

    std::vector<unsigned int> powers(stringLength);
    for (unsigned int i = 0; i < powers.size(); i++) {
        powers[i] = pow(charSet.length(), i);
    }
    
    for (unsigned int i = 0; i < vector_length; i++) {
        vec.push_back(getStringFromIndex(i, powers, charSet));
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);

    return vec;
}

std::vector<std::string> getRandomLengthExampleInput(
    unsigned int numElements,
    unsigned int minLength,
    unsigned int maxLength,
    std::string charSet
) {
    std::vector<std::string> vec;
    vec.reserve(numElements);

    for (unsigned int i = 0; i < numElements; i++) {
        unsigned int elementLength{minLength};
        if (maxLength > minLength) {
            elementLength += rand() % (maxLength - minLength);
        }
        vec.push_back(getRandomString(elementLength, charSet));
    }

    return vec;
}
