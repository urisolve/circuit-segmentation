/**
 * @file
 */

#include "common/UuidGen.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Test class of UuidGen.
 */
class UuidGenTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mUuidGen = std::make_unique<common::UuidGen>();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Number of characters using v4 method for UUID. */
    const unsigned int cNumCharsId{36};

    /** Universally Unique Identifier (UUID) generator. */
    std::unique_ptr<common::UuidGen> mUuidGen;
};

/**
 * @brief Tests that the generation of a UUID is correct.
 */
TEST_F(UuidGenTest, generatesId)
{
    const auto id{mUuidGen->generateUuid()};
    const auto idString{uuids::to_string(id)};
    std::cout << "id = " << idString << std::endl;

    EXPECT_EQ(idString.size(), cNumCharsId);
}

/**
 * @brief Tests that the generation of a string UUID is correct.
 */
TEST_F(UuidGenTest, generatesStringId)
{
    const auto id{mUuidGen->generateStringUuid()};
    std::cout << "id = " << id << std::endl;

    EXPECT_EQ(id.size(), cNumCharsId);
}

/**
 * @brief Tests that the generation of multiple UUIDs have different IDs.
 */
TEST_F(UuidGenTest, generatesMultipleIds)
{
    const auto id1{mUuidGen->generateStringUuid()};
    const auto id2{mUuidGen->generateStringUuid()};
    const auto id3{mUuidGen->generateStringUuid()};
    const auto id4{mUuidGen->generateStringUuid()};
    std::cout << "id1 = " << id1 << std::endl;
    std::cout << "id2 = " << id2 << std::endl;
    std::cout << "id3 = " << id3 << std::endl;
    std::cout << "id4 = " << id4 << std::endl;

    // Expect that IDs are different
    EXPECT_TRUE(id1 != id2);
    EXPECT_TRUE(id1 != id3);
    EXPECT_TRUE(id1 != id4);
    EXPECT_TRUE(id2 != id3);
    EXPECT_TRUE(id2 != id4);
    EXPECT_TRUE(id3 != id4);
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
