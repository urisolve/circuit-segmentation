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
class ImageReceiverTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<computerVision::MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mImageReceiver = std::make_unique<imageProcessing::ImageReceiver>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Image receiver. */
    std::unique_ptr<imageProcessing::ImageReceiver> mImageReceiver;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<computerVision::MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

/**
 * @brief Tests that when the image read is not empty, the image is received successfully.
 */
TEST_F(ImageReceiverTest, receivesImageSuccessfully)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, readImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, isImageEmpty).Times(1).WillOnce(Return(false));

    // Receive image
    EXPECT_TRUE(mImageReceiver->receiveImage());
}

/**
 * @brief Tests when the image read is empty, the image is received unsuccessfully.
 */
TEST_F(ImageReceiverTest, receivesImageUnsuccessfully)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, readImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, isImageEmpty).Times(1).WillOnce(Return(true));

    // Receive image
    EXPECT_FALSE(mImageReceiver->receiveImage());
}

/**
 * @brief Tests that the image file path is defined correctly.
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
