#include <gtest/gtest.h>
#include "example_inputs.h"

struct FixedSizeExampleInputArgs {
    unsigned int length;
    std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // FixedSizeExampleInputArgs(unsigned int length): length{length} {}
    // FixedSizeExampleInputArgs(unsigned int length, const std::string &charSet): length{length}, charSet{charSet} {}
};

class TestFixedSizeExampleInput : public testing::TestWithParam<FixedSizeExampleInputArgs> {
    protected:
        std::vector<std::string> inputData;

        void SetUp() override {
            inputData = getFixedSizeExampleInput(
                GetParam().length,
                GetParam().charSet
            );
        }

        std::vector<std::string> getSortedData() {
            std::vector<std::string> sorted = inputData;
            std::sort(sorted.begin(), sorted.end());
            return sorted;
        }
};

TEST_P(TestFixedSizeExampleInput, InputElementCount) {
    ASSERT_GT(GetParam().charSet.size(), 0);
    ASSERT_GE(GetParam().length, 0);
    ASSERT_GT(inputData.size(), 0);  // if length is 0, inputData should contain an empty string
    ASSERT_EQ(inputData.size(), pow(GetParam().charSet.length(), GetParam().length));
}

TEST_P(TestFixedSizeExampleInput, InputElementsCorrectLength) {
    bool allSameLength = true;

    for (std::string &inputElement : inputData) {
        if (inputElement.length() != GetParam().length) {
            allSameLength = false;
        }
    }

    ASSERT_TRUE(allSameLength);
}


TEST_P(TestFixedSizeExampleInput, InputElementsShuffled) {
    if (inputData.size() == 1) {
        GTEST_SKIP() << "Nothing to shuffle in a one-element vector";
    }

    std::vector<std::string> sortedData = getSortedData();

    ASSERT_EQ(sortedData.size(), inputData.size());
    int matches{0};
    for (size_t i = 0; i < inputData.size(); i++) {
        if (inputData[i] == sortedData[i]) {
            matches++;
        }
    }
    EXPECT_LT(matches, inputData.size());
}

TEST_P(TestFixedSizeExampleInput, NStartWithChar0Correct) {
    int startWithChar0{0};
    if (inputData[0].length() == 0) {
        GTEST_SKIP() << "Skipping because length < 1";
    } 
    for (std::string &s : inputData) {
        if (s[0] == GetParam().charSet[0]) {
            startWithChar0++;
        }
    }
    ASSERT_EQ(startWithChar0, pow(GetParam().charSet.length(), GetParam().length - 1));
}

INSTANTIATE_TEST_SUITE_P(
    FixedSizeExampleInputTests,
    TestFixedSizeExampleInput,
    testing::Values(
        FixedSizeExampleInputArgs{0},
        FixedSizeExampleInputArgs{2},
        FixedSizeExampleInputArgs{4},
        FixedSizeExampleInputArgs{4, "AB"}
    )
);
