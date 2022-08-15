/**
 * @file
 */

#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/SegmentationUtils.h"
#include <gtest/gtest.h>
#include <memory>
#include <utility>

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
 * @brief Tests that a bounding box is generated correctly.
 */
TEST(SegmentationUtilsTest, generatesBoundingBox)
{
    const auto mockOpenCvWrapper{std::make_shared<NiceMock<computerVision::MockOpenCvWrapper>>()};
    constexpr auto x{50};
    constexpr auto y{50};
    constexpr auto dimension{20};
    const computerVision::Rectangle rect{x, y, dimension, dimension};

    // Setup expectations and behavior
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    EXPECT_CALL(*mockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));

    // Generate bounding box
    computerVision::Contour contour{};
    computerVision::ImageMat img{};
    constexpr auto widthIncr{2};
    constexpr auto heightIncr{2};
    const auto box{schematicSegmentation::generateBoundingBox(mockOpenCvWrapper, contour, img, widthIncr, heightIncr)};

    // Check generated boc
    const auto expectedBox{
        schematicSegmentation::increaseBoundingBox(rect, widthIncr, heightIncr, imgWidth, imgHeight)};
    EXPECT_EQ(box.x, expectedBox.x);
    EXPECT_EQ(box.y, expectedBox.y);
    EXPECT_EQ(box.width, expectedBox.width);
    EXPECT_EQ(box.height, expectedBox.height);
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

/**
 * @brief Tests that the distance between points is correctly calculated.
 */
TEST(SegmentationUtilsTest, distancePoints)
{
    // Points
    const computerVision::Point pt1{0, 0};
    const computerVision::Point pt2{5, 0};
    const computerVision::Point pt3{0, 5};
    const computerVision::Point pt4{5, 5};

    // Distance
    const auto distanceP1P2{schematicSegmentation::distancePoints(pt1.x, pt1.y, pt2.x, pt2.y)};
    const auto distanceP1P3{schematicSegmentation::distancePoints(pt1.x, pt1.y, pt3.x, pt3.y)};
    const auto distanceP1P4{schematicSegmentation::distancePoints(pt1.x, pt1.y, pt4.x, pt4.y)};

    // Expectations
    const auto expectDistanceP1P2{pt2.x};
    const auto expectDistanceP1P3{pt3.y};
    const auto expectDistanceP1P4{std::sqrt(50)};

    EXPECT_DOUBLE_EQ(distanceP1P2, expectDistanceP1P2);
    EXPECT_DOUBLE_EQ(distanceP1P3, expectDistanceP1P3);
    EXPECT_DOUBLE_EQ(distanceP1P4, expectDistanceP1P4);
}

/**
 * @brief Tests that the distance between points is zero when the points are the same.
 */
TEST(SegmentationUtilsTest, distancePointsSamePoint)
{
    // Points
    const computerVision::Point pt1{5, 5};

    // Distance
    const auto distance{schematicSegmentation::distancePoints(pt1.x, pt1.y, pt1.x, pt1.y)};

    // Expectations
    const auto expectDistance{0};

    EXPECT_DOUBLE_EQ(distance, expectDistance);
}

/**
 * @brief Tests that the distance between rectangles is correctly calculated.
 */
TEST(SegmentationUtilsTest, distanceRectangles)
{
    // Rectangle
    constexpr auto x1{50};
    constexpr auto y1{50};
    constexpr auto dimension{20};
    const computerVision::Rectangle rect1{x1, y1, dimension, dimension};

    // Test cases for rectangles and expected distances
    std::vector<std::pair<computerVision::Rectangle, double>> testCases{};

    // Case: rectangle is on top of rectangle 1
    computerVision::Rectangle rect2{x1, y1 - dimension - 10, dimension, dimension};
    double expectDistance{static_cast<double>(rect1.y - (rect2.y + rect2.height))};
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on bottom of rectangle 1
    rect2.x = x1;
    rect2.y = y1 + dimension + 20;
    expectDistance = rect2.y - (rect1.y + rect1.height);
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on left of rectangle 1
    rect2.x = x1 - dimension - 15;
    rect2.y = y1;
    expectDistance = rect1.x - (rect2.x + rect2.width);
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on right of rectangle 1
    rect2.x = x1 + dimension + 25;
    rect2.y = y1;
    expectDistance = rect2.x - (rect1.x + rect1.width);
    testCases.push_back(std::pair(rect2, expectDistance));

    // Case: rectangle is on top and left of rectangle 1
    rect2.x = x1 - dimension - 15;
    rect2.y = y1 - dimension - 10;
    expectDistance
        = schematicSegmentation::distancePoints(rect2.x + rect2.width, rect2.y + rect2.height, rect1.x, rect1.y);
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on top and right of rectangle 1
    rect2.x = x1 + dimension + 25;
    rect2.y = y1 - dimension - 10;
    expectDistance
        = schematicSegmentation::distancePoints(rect2.x, rect2.y + rect2.height, rect1.x + rect1.width, rect1.y);
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on bottom and left of rectangle 1
    rect2.x = x1 - dimension - 15;
    rect2.y = y1 + dimension + 20;
    expectDistance
        = schematicSegmentation::distancePoints(rect2.x + rect2.width, rect2.y, rect1.x, rect1.y + rect1.height);
    testCases.push_back(std::pair(rect2, expectDistance));
    // Case: rectangle is on bottom and right of rectangle 1
    rect2.x = x1 + dimension + 25;
    rect2.y = y1 + dimension + 20;
    expectDistance
        = schematicSegmentation::distancePoints(rect2.x, rect2.y, rect1.x + rect1.width, rect1.y + rect1.height);
    testCases.push_back(std::pair(rect2, expectDistance));

    for (const auto& testCase : testCases) {
        // Distance
        const auto distance{schematicSegmentation::distanceRectangles(rect1, testCase.first)};
        // Expectation
        EXPECT_DOUBLE_EQ(distance, testCase.second);
    }
}

/**
 * @brief Tests that the distance between rectangles is equal to zero when the rectangles intersect.
 */
TEST(SegmentationUtilsTest, distanceRectanglesWhenIntersection)
{
    // Rectangles
    constexpr auto x1{50};
    constexpr auto y1{50};
    constexpr auto dimension{20};
    const computerVision::Rectangle rect1{x1, y1, dimension, dimension};
    // Intersection on top border
    const computerVision::Rectangle rect2{x1, y1 - dimension, dimension, dimension};
    // Intersection with some common area
    const computerVision::Rectangle rect3{x1 - dimension / 2, y1, dimension, dimension};

    // Distance
    const auto distanceRect2{schematicSegmentation::distanceRectangles(rect1, rect2)};
    const auto distanceRect3{schematicSegmentation::distanceRectangles(rect1, rect3)};

    // Expectations
    const auto expectDistance{0};

    EXPECT_DOUBLE_EQ(distanceRect2, expectDistance);
    EXPECT_DOUBLE_EQ(distanceRect3, expectDistance);
}

/**
 * @brief Tests that the distance between rectangles is equal to zero when the rectangles are the same.
 */
TEST(SegmentationUtilsTest, distanceRectanglesSameRectangle)
{
    // Rectangles
    constexpr auto x1{50};
    constexpr auto y1{50};
    constexpr auto dimension{20};
    const computerVision::Rectangle rect1{x1, y1, dimension, dimension};

    // Distance
    const auto distance{schematicSegmentation::distanceRectangles(rect1, rect1)};

    // Expectations
    const auto expectDistance{0};

    EXPECT_DOUBLE_EQ(distance, expectDistance);
}
