#include <gtest/gtest.h>
#include "example_inputs.h"
#include "search_method.h"

struct TreeSearchAlgorithmArgs {
    std::vector<std::string> inputData;
    std::string searchString;
    size_t nThreads = 4;
};

class TestTreeSearchAlgorithm : public testing::TestWithParam<TreeSearchAlgorithmArgs> {
    protected:
        std::unique_ptr<ParallelLinearSearchAlgorithm> baseline;
        std::unique_ptr<TreeSearchAlgorithm> method;

        void SetUp() override {
            baseline = std::make_unique<ParallelLinearSearchAlgorithm>(GetParam().inputData, GetParam().nThreads);
            method = std::make_unique<TreeSearchAlgorithm>(GetParam().inputData, GetParam().nThreads);
        }
};

TEST_P(TestTreeSearchAlgorithm, CompareToBaseline) {
    std::vector<std::string> baselineResults = baseline->search(GetParam().searchString);
    std::vector<std::string> methodResults = method->search(GetParam().searchString);

    ASSERT_EQ(baselineResults.size(), methodResults.size());

    std::set<std::string> baselineResultsSet(baselineResults.begin(), baselineResults.end());
    std::set<std::string> methodResultsSet(methodResults.begin(), methodResults.end());
    ASSERT_EQ(baselineResultsSet, methodResultsSet);
}

INSTANTIATE_TEST_SUITE_P(
    TreeSearchAlgorithmTests,
    TestTreeSearchAlgorithm,
    testing::Values(
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(4), "ABCD"},
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(4), "EFG"},
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(4), "HI"},
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(4), "J"},
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(4), ""},
        TreeSearchAlgorithmArgs{getFixedSizeExampleInput(3), "ABCD"},
        TreeSearchAlgorithmArgs{getRandomLengthExampleInput(1000, 2, 10), "A"}
    )
);
