/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include <algorithm>
#include <cmath>
#include <utility>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Increases size of bounding box and centers it.
 *
 * @param box Bounding box.
 * @param widthIncr Increment for width.
 * @param heightIncr Increment for height.
 * @param widthMax Maximum width (x coordinate + box width).
 * @param heightMax Maximum height (y coordinate + box height).
 *
 * @return Enlarged size bounding box.
 */
inline const computerVision::Rectangle increaseBoundingBox(const computerVision::Rectangle& box,
                                                           const int& widthIncr,
                                                           const int& heightIncr,
                                                           const int& widthMax,
                                                           const int& heightMax)
{
    // Axis
    auto x{box.x - widthIncr / 2};  // Truncated
    x = x < 0 ? 0 : x;              // Cannot be negative
    auto y{box.y - heightIncr / 2}; // Truncated
    y = y < 0 ? 0 : y;              // Cannot be negative

    // Dimensions
    auto width{box.width + widthIncr};
    if ((x + width) > widthMax) {
        width = widthMax - x;
    }
    auto height{box.height + heightIncr};
    if ((y + height) > heightMax) {
        height = heightMax - y;
    }

    return computerVision::Rectangle{x, y, width, height};
}

/**
 * @brief Generates a bounding box for a contour.
 *
 * @param openCvWrapper OpenCV wrapper.
 * @param contour Contour to get a bounding box.
 * @param image Image used in processing.
 * @param widthIncr Increment on bounding box width.
 * @param heightIncr Increment on bounding box height.
 *
 * @return Bounding box for the contour.
 */
inline computerVision::Rectangle
    generateBoundingBox(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                        const computerVision::Contour& contour,
                        computerVision::ImageMat& image,
                        const int& widthIncr,
                        const int& heightIncr)
{
    const auto imgWidth{openCvWrapper->getImageWidth(image)};
    const auto imgHeight{openCvWrapper->getImageHeight(image)};

    // Bounding rectangle
    auto rect{openCvWrapper->boundingRect(contour)};

    // Increase dimensions because bounding boxes may not completely enclose the contour
    rect = increaseBoundingBox(rect, widthIncr, heightIncr, imgWidth, imgHeight);

    return rect;
}

/**
 * @brief Finds the extreme points for the axis selected.
 *
 * Way that the points are returned:
 * - For x axis: {leftMost, rightMost}
 * - For y axis: {topMost, bottomMost}
 *
 * If there are points with the same coordinates:
 * - leftMost: the most is the first point compared
 * - rightMost: the most is the last point compared
 * - topMost: the most is the first point compared
 * - bottomMost: the most is the last point compared
 *
 * @param points Vector of points.
 * @param axisSel Selected X axis when true, y axis when false.
 *
 * @return Pair of extreme points.
 *
 * @note The top is the point with smaller y coordinate.
 */
inline const std::pair<computerVision::Point, computerVision::Point>
    findExtremePoints(const std::vector<computerVision::Point>& points, const bool axisSel)
{
    std::pair<computerVision::Point, computerVision::Point> pairPoints;

    if (axisSel) {
        // X axis
        auto pair = std::minmax_element(
            points.begin(), points.end(), [](const computerVision::Point& a, const computerVision::Point& b) {
                return a.x < b.x;
            });

        // Make pair {leftMost, rightMost}
        pairPoints = std::make_pair<computerVision::Point, computerVision::Point>(
            computerVision::Point(pair.first->x, pair.first->y), computerVision::Point(pair.second->x, pair.second->y));
    } else {
        // Y axis
        auto pair = std::minmax_element(
            points.begin(), points.end(), [](const computerVision::Point& a, const computerVision::Point& b) {
                return a.y < b.y;
            });

        // Make pair {topMost, bottomMost}
        pairPoints = std::make_pair<computerVision::Point, computerVision::Point>(
            computerVision::Point(pair.first->x, pair.first->y), computerVision::Point(pair.second->x, pair.second->y));
    }

    return pairPoints;
}

/**
 * @brief Calculates the distance between two points.
 *
 * @param x1 X coordinate of point 1.
 * @param y1 Y coordinate of point 1.
 * @param x2 X coordinate of point 2.
 * @param y2 Y coordinate of point 2.
 *
 * @return Distance between the two points.
 */
inline const double distancePoints(const int& x1, const int& y1, const int& x2, const int& y2)
{
    return std::sqrt(static_cast<double>(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2)));
}

/**
 * @brief Calculates the distance between two rectangles.
 *
 * @param rect1 Rectangle 1.
 * @param rect2 Rectangle 2.
 *
 * @return Distance between the two rectangles.
 *
 * @note This method does not account for the rotation of the rectangles.
 */
inline const double distanceRectangles(const computerVision::Rectangle& rect1, const computerVision::Rectangle& rect2)
{
    // Corners
    const computerVision::Point leftTop1{rect1.x, rect1.y};
    const computerVision::Point rightBottom1{rect1.x + rect1.width, rect1.y + rect1.height};
    const computerVision::Point leftTop2{rect2.x, rect2.y};
    const computerVision::Point rightBottom2{rect2.x + rect2.width, rect2.y + rect2.height};

    // Positions for rectangle 2 relative to rectangle 1
    const auto left{rightBottom2.x < leftTop1.x};
    const auto right{leftTop2.x > rightBottom1.x};
    const auto top{rightBottom2.y < leftTop1.y};
    const auto bottom{leftTop2.y > rightBottom1.y};

    if (top && left) {
        return distancePoints(rightBottom2.x, rightBottom2.y, leftTop1.x, leftTop1.y);
    } else if (top && right) {
        return distancePoints(leftTop2.x, rightBottom2.y, rightBottom1.x, leftTop1.y);
    } else if (bottom && left) {
        return distancePoints(rightBottom2.x, leftTop2.y, leftTop1.x, rightBottom1.y);
    } else if (bottom && right) {
        return distancePoints(leftTop2.x, leftTop2.y, rightBottom1.x, rightBottom1.y);
    } else if (left) {
        return leftTop1.x - rightBottom2.x;
    } else if (right) {
        return leftTop2.x - rightBottom1.x;
    } else if (top) {
        return leftTop1.y - rightBottom2.y;
    } else if (bottom) {
        return leftTop2.y - rightBottom1.y;
    }

    // Intersection case
    return 0;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
