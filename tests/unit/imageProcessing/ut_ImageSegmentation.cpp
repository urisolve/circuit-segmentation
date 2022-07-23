/**
 * @file
 */

#include "imageProcessing/ImageSegmentation.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "mocks/schematicSegmentation/MockComponentSegmentation.h"
#include "mocks/schematicSegmentation/MockConnectionSegmentation.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;
using namespace circuitSegmentation::schematicSegmentation;

/**
 * @brief Test class of ImageSegmentation.
 */
class ImageSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);
        mMockComponentSegmentation = std::make_shared<NiceMock<MockComponentSegmentation>>(nullptr, nullptr);
        mMockConnectionSegmentation = std::make_shared<NiceMock<MockConnectionSegmentation>>(nullptr, nullptr);

        mImageSegmentation = std::make_unique<imageProcessing::ImageSegmentation>(
            mMockOpenCvWrapper, mLogger, mMockComponentSegmentation, mMockConnectionSegmentation, true);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Image segmentation. */
    std::unique_ptr<imageProcessing::ImageSegmentation> mImageSegmentation;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Component segmentation. */
    std::shared_ptr<NiceMock<MockComponentSegmentation>> mMockComponentSegmentation;
    /** Connection segmentation. */
    std::shared_ptr<NiceMock<MockConnectionSegmentation>> mMockConnectionSegmentation;
};

/**
 * @brief Tests that segmentation occurs successfully.
 */
TEST_F(ImageSegmentationTest, segmentsSuccessfully)
{
    const std::vector<circuit::Component> emptyComponents{};
    const std::vector<circuit::Connection> emptyConnections{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentSegmentation, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentSegmentation, getDetectedComponents)
        .Times(1)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionSegmentation, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionSegmentation, getDetectedConnections)
        .Times(1)
        .WillRepeatedly([&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
    EXPECT_CALL(*mMockComponentSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockComponentSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(true));

    // Segment image
    ImageMat image{};
    ASSERT_TRUE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no components detected.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoComponents)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentSegmentation, detectComponents).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockComponentSegmentation, getDetectedComponents).Times(0);
    EXPECT_CALL(*mMockConnectionSegmentation, detectConnections).Times(0);
    EXPECT_CALL(*mMockConnectionSegmentation, getDetectedConnections).Times(0);
    EXPECT_CALL(*mMockComponentSegmentation, detectComponentConnections).Times(0);
    EXPECT_CALL(*mMockComponentSegmentation, updateDetectedComponents).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no connections detected.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoConnections)
{
    const std::vector<circuit::Component> emptyComponents{};
    const std::vector<circuit::Connection> emptyConnections{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentSegmentation, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentSegmentation, getDetectedComponents)
        .Times(1)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionSegmentation, detectConnections).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockConnectionSegmentation, getDetectedConnections).Times(0);
    EXPECT_CALL(*mMockComponentSegmentation, detectComponentConnections).Times(0);
    EXPECT_CALL(*mMockComponentSegmentation, updateDetectedComponents).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no components detected after update.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoComponentsAfterUpdate)
{
    const std::vector<circuit::Component> emptyComponents{};
    const std::vector<circuit::Connection> emptyConnections{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentSegmentation, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentSegmentation, getDetectedComponents)
        .Times(1)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionSegmentation, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionSegmentation, getDetectedConnections)
        .Times(1)
        .WillRepeatedly([&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
    EXPECT_CALL(*mMockComponentSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockComponentSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(false));

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that the flag to save images is defined correctly.
 */
TEST_F(ImageSegmentationTest, setsSaveImages)
{
    const auto saveImages{true};
    mImageSegmentation->setSaveImages(saveImages);

    EXPECT_EQ(saveImages, mImageSegmentation->getSaveImages());
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
