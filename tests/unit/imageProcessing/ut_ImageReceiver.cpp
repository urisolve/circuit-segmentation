/**
 * @file
 */

#include "imageProcessing/ImageReceiver.h"
#include "logging/MockLogger.h"
#include <gtest/gtest.h>
#include <memory>
#include <opencv2/core.hpp>
#include <string>

/**
 * @brief Test class of ImageReceiver.
 */
class ImageReceiverTest : public ::testing::Test
{
protected:
    /** Existent image file path to be used in tests. */
    const std::string cExistentImageFilePath{std::string(TESTS_DATA_PATH) + "circuit-comp-1.png"};
    /** Nonexistent image file path to be used in tests. */
    const std::string cNonExistentImageFilePath{std::string(TESTS_DATA_PATH) + "nonexistent.png"};

    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockLogger = std::make_shared<testing::NiceMock<circuitSegmentation::logging::MockLogger>>(std::cout);
        mImageReceiver = std::make_unique<circuitSegmentation::imageProcessing::ImageReceiver>(mMockLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /** Image receiver. */
    std::unique_ptr<circuitSegmentation::imageProcessing::ImageReceiver> mImageReceiver;
    /** Logger. */
    std::shared_ptr<testing::NiceMock<circuitSegmentation::logging::MockLogger>> mMockLogger;
};

/**
 * @brief Tests if image is received successfully.
 *
 * This test uses a correct image file path for image receiver.
 */
TEST_F(ImageReceiverTest, receivesImageSuccessfully)
{
    const auto filePath{cExistentImageFilePath};
    mImageReceiver->setImageFilePath(filePath);

    // Verify if image was received successfully
    const auto isImageOk = mImageReceiver->receiveImage();

    EXPECT_EQ(true, isImageOk);
}

/**
 * @brief Tests if image is received unsuccessfully.
 *
 * This test uses a nonexistent image file path for image receiver.
 */
TEST_F(ImageReceiverTest, receivesImageUnsuccessfully)
{
    const auto filePath{cNonExistentImageFilePath};
    mImageReceiver->setImageFilePath(filePath);

    // Verify if image was received unsuccessfully
    const auto isImageOk = mImageReceiver->receiveImage();
    const cv::Mat imageExpected = mImageReceiver->getImageReceived();

    EXPECT_EQ(false, isImageOk);
    EXPECT_EQ(true, imageExpected.empty());
}

/**
 * @brief Tests the image file path setted.
 */
TEST_F(ImageReceiverTest, setsImageFilePath)
{
    const auto filePath{cExistentImageFilePath};
    mImageReceiver->setImageFilePath(filePath);

    EXPECT_EQ(filePath, mImageReceiver->getImageFilePath());
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
