/**
 * @file
 */

#include "circuit/Component.h"
#include <gtest/gtest.h>
#include <vector>

using namespace circuitSegmentation;
using namespace circuitSegmentation::circuit;

/**
 * @brief Test class of Component.
 */
class ComponentTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mComponent = std::make_unique<Component>();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Circuit component. */
    std::unique_ptr<Component> mComponent;
};

/**
 * @brief Tests that the component bounding box is set correctly.
 */
TEST_F(ComponentTest, setsComponentBoundingBox)
{
    // Rectangle parameters
    const auto x{0};
    const auto y{0};
    const auto width{100};
    const auto height{100};
    const computerVision::Rectangle box{x, y, width, height};

    // Set box
    mComponent->setBoundingBox(box);

    // Get box
    const auto expectedBox{mComponent->getBoundingBox()};

    EXPECT_EQ(expectedBox.x, box.x);
    EXPECT_EQ(expectedBox.y, box.y);
    EXPECT_EQ(expectedBox.width, box.width);
    EXPECT_EQ(expectedBox.height, box.height);
}

/**
 * @brief Tests that the nodes to which component is connected are set correctly.
 */
TEST_F(ComponentTest, setsComponentNodes)
{
    // Node parameters
    Node node{};
    const computerVision::Point p1{0, 0};
    const computerVision::Point p2{99, 0};
    Wire wire{p1, p2};
    node.setWire(wire);

    // Nodes
    const std::vector<Node> nodes{node};

    // Set nodes
    mComponent->setNodes(nodes);

    // Get nodes
    const auto expectNodes{mComponent->getNodes()};
    const auto expectNode{expectNodes.back()};
    const auto expectWire{expectNode.getWire()};
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
