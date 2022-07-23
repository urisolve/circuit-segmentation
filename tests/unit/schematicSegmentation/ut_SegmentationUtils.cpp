/**
 * @file
 */

#include "schematicSegmentation/SegmentationUtils.h"
#include <gtest/gtest.h>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Tests that the bounding box size increases and is centered.
 */
TEST(SegmentationUtilsTest, increasesBoundingBox)
{
    const computerVision::Rectangle box{20, 20, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        schematicSegmentation::increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    const auto expectedX{box.x - widthIncr / 2};
    const auto expectedY{box.y - heightIncr / 2};
    const auto expectedWidth{box.width + widthIncr};
    const auto expectedHeight{box.height + heightIncr};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

/**
 * @brief Tests that the bounding box size increases for the case when it is on top left corner.
 */
TEST(SegmentationUtilsTest, increasesBoundingBoxTopLeftCorner)
{
    const computerVision::Rectangle box{0, 0, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        schematicSegmentation::increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    constexpr auto expectedX{0};
    constexpr auto expectedY{0};
    const auto expectedWidth{box.width + widthIncr};
    const auto expectedHeight{box.height + heightIncr};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

/**
 * @brief Tests that the bounding box size increases for the case when it is on bottom right corner.
 */
TEST(SegmentationUtilsTest, increasesBoundingBoxBottomRightCorner)
{
    const computerVision::Rectangle box{90, 90, 10, 10};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        schematicSegmentation::increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    const auto expectedX{box.x - widthIncr / 2};
    const auto expectedY{box.y - heightIncr / 2};
    const auto expectedWidth{widthMax - expectedX};
    const auto expectedHeight{heightMax - expectedY};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

/**
 * @brief Tests that the extreme points are found.
 */
TEST(SegmentationUtilsTest, findsExtremePoints)
{
    // Points
    const computerVision::Point pt1{0, 0};
    const computerVision::Point pt2{1, 0};
    const computerVision::Point pt3{0, 1};
    const computerVision::Point pt4{1, 1};
    const std::vector<computerVision::Point> points{pt1, pt2, pt3, pt4};

    // Find extreme points
    const auto extremeX{schematicSegmentation::findExtremePoints(points, true)};
    const auto extremeY{schematicSegmentation::findExtremePoints(points, false)};

    // Expected extreme points
    const auto leftPoint{pt1};
    const auto rightPoint{pt4};
    const auto topPoint{pt1};
    const auto bottomPoint{pt4};

    EXPECT_EQ(leftPoint, extremeX.first);
    EXPECT_EQ(rightPoint, extremeX.second);
    EXPECT_EQ(topPoint, extremeY.first);
    EXPECT_EQ(bottomPoint, extremeY.second);
}
