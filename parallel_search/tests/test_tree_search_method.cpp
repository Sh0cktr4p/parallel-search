#include <gtest/gtest.h>
#include "example_inputs.h"
#include "search_method.h"


struct TreeSearchMethodArgs {
    std::vector<std::string> inputData;
    std::string searchString;
};

class TestTreeSearchMethod : public testing::TestWithParam<TreeSearchMethodArgs> {
    protected:
        std::unique_ptr<LinearSearchMethod> baseline;
        std::unique_ptr<TreeSearchMethod> method;

        void SetUp() override {
            baseline = std::make_unique<LinearSearchMethod>(GetParam().inputData);
            method = std::make_unique<TreeSearchMethod>(GetParam().inputData);
        }
};

TEST_P(TestTreeSearchMethod, CompareToBaseline) {
    std::vector<std::string> baselineResults = baseline->search(GetParam().searchString);
    std::vector<std::string> methodResults = method->search(GetParam().searchString);

    ASSERT_EQ(baselineResults.size(), methodResults.size());

    std::set<std::string> baselineResultsSet(baselineResults.begin(), baselineResults.end());
    std::set<std::string> methodResultsSet(methodResults.begin(), methodResults.end());
    ASSERT_EQ(baselineResultsSet, methodResultsSet);
}

INSTANTIATE_TEST_SUITE_P(
    TreeSearchMethodTests,
    TestTreeSearchMethod,
    testing::Values(
        TreeSearchMethodArgs{getFixedSizeExampleInput(4), "ABCD"},
        TreeSearchMethodArgs{getFixedSizeExampleInput(4), "EFG"},
        TreeSearchMethodArgs{getFixedSizeExampleInput(4), "HI"},
        TreeSearchMethodArgs{getFixedSizeExampleInput(4), "J"},
        TreeSearchMethodArgs{getFixedSizeExampleInput(4), ""},
        TreeSearchMethodArgs{getFixedSizeExampleInput(3), "ABCD"},
        TreeSearchMethodArgs{getRandomLengthExampleInput(1000, 2, 10), "A"}
    )
);
