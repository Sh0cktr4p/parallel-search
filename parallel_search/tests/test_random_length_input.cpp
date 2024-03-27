#include <gtest/gtest.h>
#include "example_inputs.h"

struct RandomLengthExampleInputArgs {
    unsigned int numElements;
    unsigned int minLength;
    unsigned int maxLength;
    std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

class TestRandomLengthExampleInput : public testing::TestWithParam<RandomLengthExampleInputArgs> {
    protected:
        std::vector<std::string> inputData;

        void SetUp() override {
            inputData = getRandomLengthExampleInput(
                GetParam().numElements,
                GetParam().minLength,
                GetParam().maxLength,
                GetParam().charSet
            );
        }

        std::vector<std::string> getSortedData() {
            std::vector<std::string> sorted = inputData;
            std::sort(sorted.begin(), sorted.end());
            return sorted;
        }
};

TEST_P(TestRandomLengthExampleInput, InputElementCount) {
    ASSERT_EQ(inputData.size(), GetParam().numElements);
}

TEST_P(TestRandomLengthExampleInput, InputElementLength) {
    bool noneTooShort = true;
    bool noneTooLong = true;

    for (std::string &element : inputData) {
        if (element.length() < GetParam().minLength) {
            noneTooShort = false;
        }
        if (element.length() > GetParam().maxLength) {
            noneTooLong = false;
        } 
    }

    ASSERT_TRUE(noneTooShort);
    ASSERT_TRUE(noneTooLong);
}

TEST_P(TestRandomLengthExampleInput, InputElementsUnordered) {
    if (inputData.size() == 1) {
        GTEST_SKIP() << "Nothing to shuffle in a one-element vector";
    }

    std::vector<std::string> sortedData = getSortedData();

    ASSERT_EQ(inputData.size(), sortedData.size());
    int matches{0};
    for (size_t i = 0; i < inputData.size(); i++) {
        if (inputData[i] == sortedData[i]) {
            matches++;
        }
    }
    EXPECT_LT(matches, inputData.size());
}

INSTANTIATE_TEST_SUITE_P(
    RandomLengthExampleInputTests,
    TestRandomLengthExampleInput,
    testing::Values(
        RandomLengthExampleInputArgs{100, 2, 10},
        RandomLengthExampleInputArgs{100, 5, 5},
        RandomLengthExampleInputArgs{100, 2, 10, "AB"}
    )
);