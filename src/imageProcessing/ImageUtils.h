/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include <algorithm>
#include <utility>

namespace circuitSegmentation {
namespace imageProcessing {

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
 * @note The top is the points with smaller y coordinate.
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

} // namespace imageProcessing
} // namespace circuitSegmentation
