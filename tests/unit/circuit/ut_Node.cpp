/**
 * @file
 */

#include "circuit/Node.h"
#include <gtest/gtest.h>
#include <vector>

using namespace circuitSegmentation;
using namespace circuitSegmentation::circuit;

/**
 * @brief Test class of Node.
 */
class NodeTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mNode = std::make_unique<Node>();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Circuit node. */
    std::unique_ptr<Node> mNode;
};

/**
 * @brief Tests that the node wire is set correctly.
 */
TEST_F(NodeTest, setsNodeWire)
{
    // Wire parameters
    const computerVision::Point p1{0, 0};
    const computerVision::Point p2{99, 0};
    Wire wire{p1, p2};

    // Set wire
    mNode->setWire(wire);

    // Get wire
    const auto expectWire{mNode->getWire()};
    const auto expectP1{expectWire.at(0)};
    const auto expectP2{expectWire.at(1)};

    EXPECT_EQ(expectP1.x, p1.x);
    EXPECT_EQ(expectP1.y, p1.y);
    EXPECT_EQ(expectP2.x, p2.x);
    EXPECT_EQ(expectP2.y, p2.y);
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
