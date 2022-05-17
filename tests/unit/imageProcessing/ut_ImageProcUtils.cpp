/**
 * @file
 */

#include "imageProcessing/ImageProcUtils.h"
#include <gtest/gtest.h>
#include <opencv2/core.hpp>

/**
 * @brief Test class of ImageProcUtils.
 */
class ImageProcUtilsTest : public ::testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mImageProcUtils = std::make_unique<circuitSegmentation::imageProcessing::ImageProcUtils>();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /** Image processing utils. */
    std::unique_ptr<circuitSegmentation::imageProcessing::ImageProcUtils> mImageProcUtils;
    /** Image to be used in tests. */
    cv::Mat mTestImage{300, 300, CV_8UC3, cv::Scalar(128, 128, 128)};
};

/**
 * @brief Tests that the showImage() method does not throw.
 */
TEST_F(ImageProcUtilsTest, DISABLED_showImageDoesNotThrow)
{
    EXPECT_NO_THROW(mImageProcUtils->showImage("Test image", mTestImage, 1));
}

/**
 * @brief Tests if image is written successfully.
 */
TEST_F(ImageProcUtilsTest, writesImageSuccessfully)
{
    const auto fileName{"test_image.png"};

    // Write image
    const auto result = mImageProcUtils->writeImage(fileName, mTestImage);

    EXPECT_EQ(true, result);
}

/**
 * @brief Tests if image is written unsuccessfully when it is used an incorrect file name.
 */
TEST_F(ImageProcUtilsTest, writesImageUnsuccessfully)
{
    const auto fileName{""};

    // Write image
    const auto result = mImageProcUtils->writeImage(fileName, mTestImage);

    EXPECT_EQ(false, result);
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
