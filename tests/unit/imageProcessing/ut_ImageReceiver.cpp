/**
 * @file
 */

#include "imageProcessing/ImageReceiver.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Test class of ImageReceiver.
 */
class ImageReceiverTest : public ::testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NaggyMock<computerVision::MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mImageReceiver = std::make_unique<imageProcessing::ImageReceiver>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /** Image receiver. */
    std::unique_ptr<imageProcessing::ImageReceiver> mImageReceiver;
    /** OpenCV wrapper. */
    std::shared_ptr<NaggyMock<computerVision::MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

/**
 * @brief Tests that when the image read is not empty, the image is received successfully.
 */
TEST_F(ImageReceiverTest, receivesImageSuccessfully)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, isImageEmpty(_)).WillByDefault(Return(false));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, readImage(_)).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, isImageEmpty(_)).Times(1);

    // Receive image
    EXPECT_EQ(true, mImageReceiver->receiveImage());
}

/**
 * @brief Tests when the image read is empty, the image is received unsuccessfully.
 */
TEST_F(ImageReceiverTest, receivesImageUnsuccessfully)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, isImageEmpty(_)).WillByDefault(Return(true));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, readImage(_)).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, isImageEmpty(_)).Times(1);

    // Receive image
    EXPECT_EQ(false, mImageReceiver->receiveImage());
}

/**
 * @brief Tests that the the image file path is defined correctly.
 */
TEST_F(ImageReceiverTest, setsImageFilePath)
{
    const auto filePath{"image.png"};
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
