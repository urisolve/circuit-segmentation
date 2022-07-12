/**
 * @file
 */

#include "common/RandomNumberGen.h"
#include <gtest/gtest.h>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Test class of RandomNumberGen.
 */
class RandomNumberGenTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mRandomNumberGen = std::make_unique<common::RandomNumberGen<int>>(cMinRandom, cMaxRandom);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Minimum random number to be used in tests. */
    const int cMinRandom{0};
    /** Maximum random number to be used in tests. */
    const int cMaxRandom{10};

    /** Simple random number generator. */
    std::unique_ptr<common::RandomNumberGen<int>> mRandomNumberGen;
};

/**
 * @brief Tests that the generation of a random number is inside of the range specified.
 */
TEST_F(RandomNumberGenTest, randomInRange)
{
    EXPECT_LE(cMinRandom, mRandomNumberGen->randomNumber());
    EXPECT_GE(cMaxRandom, mRandomNumberGen->randomNumber());
}

/**
 * @brief Tests that the generation of a random number is always the same when the minimum and maximum for the range is
 * equal.
 */
TEST_F(RandomNumberGenTest, randomEqualsRange)
{
    constexpr auto number{10};

    common::RandomNumberGen<int> rng{number, number};

    EXPECT_EQ(number, rng.randomNumber());
}

// /**
//  * @brief Main function.
//  *
//  * @param argc Number of command line arguments.
//  * @param argv Command line arguments.
//  *
//  * @return 0 on success, 1 on failure.
//  */
// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
