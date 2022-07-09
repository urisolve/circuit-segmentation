/**
 * @file
 */

#include "computerVision/OpenCvWrapper.h"
#include "imageProcessing/ImageUtils.h"
#include <gtest/gtest.h>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Tests that the extreme points are found.
 */
TEST(ImageUtilsTest, findsExtremePoints)
{
    // Points
    const computerVision::Point pt1{0, 0};
    const computerVision::Point pt2{1, 0};
    const computerVision::Point pt3{0, 1};
    const computerVision::Point pt4{1, 1};
    const std::vector<computerVision::Point> points{pt1, pt2, pt3, pt4};

    // Find extreme points
    const auto extremeX{imageProcessing::findExtremePoints(points, true)};
    const auto extremeY{imageProcessing::findExtremePoints(points, false)};

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
