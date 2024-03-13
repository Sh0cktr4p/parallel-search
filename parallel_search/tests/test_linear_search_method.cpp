#include <gtest/gtest.h>
#include "example_inputs.h"
#include "search_method.h"


struct LinearSearchMethodArgs {
    std::vector<std::string> inputData;
};


class TestLinearSearchMethod : public testing::TestWithParam<LinearSearchMethodArgs> {
    protected:
        std::unique_ptr<LinearSearchMethod> method;

        void SetUp() override {
            method = std::make_unique<LinearSearchMethod>(GetParam().inputData);
        }
};

TEST_P(TestLinearSearchMethod, FindSingleInputElement) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }
    std::vector<std::string> result = method->search(GetParam().inputData[0]);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], GetParam().inputData[0]);
}

TEST_P(TestLinearSearchMethod, FindEmptyString) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }
    std::vector<std::string> result = method->search("");
    ASSERT_EQ(result.size(), GetParam().inputData.size());
    std::set<std::string> inputDataSet(GetParam().inputData.begin(), GetParam().inputData.end());
    std::set<std::string> outputDataSet(result.begin(), result.end());

    ASSERT_EQ(inputDataSet, outputDataSet);
}

TEST_P(TestLinearSearchMethod, FindSubstring) {
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
    ASSERT_GE(method->search(substr).size(), 1);
}

TEST_P(TestLinearSearchMethod, SearchSuperstring) {
    if (GetParam().inputData.size() == 0) {
        GTEST_SKIP() << "Cannot find anything in empty input data";
    }

    std::string superstr = GetParam().inputData[0] + "A";
    ASSERT_EQ(method->search(superstr).size(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    LinearSearchMethodTests,
    TestLinearSearchMethod,
    testing::Values(
        LinearSearchMethodArgs{getFixedSizeExampleInput(0)},
        LinearSearchMethodArgs{getFixedSizeExampleInput(4)},
        LinearSearchMethodArgs{getRandomLengthExampleInput(100, 2, 10)}
    )
);

TEST(LinearSearchMethodTests, TestOnFixedSizeInput) {
    std::vector<std::string> data = getFixedSizeExampleInput(4);
    LinearSearchMethod s(data);

    ASSERT_EQ(s.search("ABBA").size(), 1);
    ASSERT_EQ(s.search("ABBA")[0], "ABBA");

    ASSERT_EQ(s.search("ABB").size(), 26);
    ASSERT_EQ(s.search("AB").size(), 26 * 26);
    ASSERT_EQ(s.search("A").size(), 26 * 26 * 26);
}