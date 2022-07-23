/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include <algorithm>
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

} // namespace schematicSegmentation
} // namespace circuitSegmentation
