#include <gtest/gtest.h>
#include "example_inputs.h"
#include "search_method.h"


struct LinearSearchAlgorithmArgs {
    std::vector<std::string> inputData;
    size_t nThreads = 1;
};


class TestLinearSearchAlgorithm : public testing::TestWithParam<LinearSearchAlgorithmArgs> {
    protected:
        std::unique_ptr<ParallelLinearSearchAlgorithm> algorithm;

        void SetUp() override {
            algorithm = std::make_unique<ParallelLinearSearchAlgorithm>(GetParam().inputData, GetParam().nThreads);
        }
};

TEST_P(TestLinearSearchAlgorithm, FindSingleInputElement) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }
    std::vector<std::string> result = algorithm->search(GetParam().inputData[0]);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], GetParam().inputData[0]);
}

TEST_P(TestLinearSearchAlgorithm, FindEmptyString) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }
    std::vector<std::string> result = algorithm->search("");
    ASSERT_EQ(result.size(), GetParam().inputData.size());
    std::set<std::string> inputDataSet(GetParam().inputData.begin(), GetParam().inputData.end());
    std::set<std::string> outputDataSet(result.begin(), result.end());

    ASSERT_EQ(inputDataSet, outputDataSet);
}

TEST_P(TestLinearSearchAlgorithm, FindSubstring) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }

    // Find an element with two characters
    std::string element = "";

    for (const std::string &s : GetParam().inputData) {
        if (s.length() > 1) {
            element = s;
        }
    }

    if (element == "") {
        GTEST_SKIP() << "No element with length > 1 found";
    }
    std::string substr = element.substr(0, element.length() - 1);
    ASSERT_GE(algorithm->search(substr).size(), 1);
}

TEST_P(TestLinearSearchAlgorithm, SearchSuperstring) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }

    std::string superstr = GetParam().inputData[0] + "A";
    ASSERT_EQ(algorithm->search(superstr).size(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    LinearSearchAlgorithmTests,
    TestLinearSearchAlgorithm,
    testing::Values(
        LinearSearchAlgorithmArgs{getFixedSizeExampleInput(0)},
        LinearSearchAlgorithmArgs{getFixedSizeExampleInput(4)},
        LinearSearchAlgorithmArgs{getRandomLengthExampleInput(100, 2, 10)}
    )
);

TEST(LinearSearchAlgorithmTests, TestOnFixedSizeInput) {
    std::vector<std::string> data = getFixedSizeExampleInput(4);
    ParallelLinearSearchAlgorithm s(data, 1);

    ASSERT_EQ(s.search("ABBA").size(), 1);
    ASSERT_EQ(s.search("ABBA")[0], "ABBA");

    ASSERT_EQ(s.search("ABB").size(), 26);
    ASSERT_EQ(s.search("AB").size(), 26 * 26);
    ASSERT_EQ(s.search("A").size(), 26 * 26 * 26);
}