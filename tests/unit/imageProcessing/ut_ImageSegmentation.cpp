/**
 * @file
 */

#include "imageProcessing/ImageSegmentation.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "mocks/schematicSegmentation/MockComponentDetection.h"
#include "mocks/schematicSegmentation/MockConnectionDetection.h"
#include "mocks/schematicSegmentation/MockSchematicSegmentation.h"
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
        mMockComponentDetection = std::make_shared<NiceMock<MockComponentDetection>>(nullptr, nullptr);
        mMockConnectionDetection = std::make_shared<NiceMock<MockConnectionDetection>>(nullptr, nullptr);
        mMockSchematicSegmentation = std::make_shared<NiceMock<MockSchematicSegmentation>>(nullptr, nullptr);

        mImageSegmentation = std::make_unique<imageProcessing::ImageSegmentation>(mMockOpenCvWrapper,
                                                                                  mLogger,
                                                                                  mMockComponentDetection,
                                                                                  mMockConnectionDetection,
                                                                                  mMockSchematicSegmentation,
                                                                                  true);
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
    /** Component detection. */
    std::shared_ptr<NiceMock<MockComponentDetection>> mMockComponentDetection;
    /** Connection detection. */
    std::shared_ptr<NiceMock<MockConnectionDetection>> mMockConnectionDetection;
    /** Schematic segmentation. */
    std::shared_ptr<NiceMock<MockSchematicSegmentation>> mMockSchematicSegmentation;
};

/**
 * @brief Tests that segmentation occurs successfully.
 */
TEST_F(ImageSegmentationTest, segmentsSuccessfully)
{
    const std::vector<circuit::Component> emptyComponents{};
    const std::vector<circuit::Connection> emptyConnections{};
    const std::vector<circuit::Node> emptyNodes{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents)
        .Times(3)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, getDetectedConnections)
        .Times(1)
        .WillRepeatedly([&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
    EXPECT_CALL(*mMockConnectionDetection, getDetectedNodes)
        .Times(1)
        .WillRepeatedly([&emptyNodes]() -> const std::vector<circuit::Node>& { return emptyNodes; });
    EXPECT_CALL(*mMockSchematicSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(true));

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
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents).Times(0);

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

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents)
        .Times(1)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no connections/nodes detected.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoConnectionsNodes)
{
    const std::vector<circuit::Component> emptyComponents{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents)
        .Times(2)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockSchematicSegmentation, detectComponentConnections).Times(0);

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
    const std::vector<circuit::Node> emptyNodes{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents)
        .Times(3)
        .WillRepeatedly([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, getDetectedConnections)
        .Times(1)
        .WillRepeatedly([&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
    EXPECT_CALL(*mMockConnectionDetection, getDetectedNodes)
        .Times(1)
        .WillRepeatedly([&emptyNodes]() -> const std::vector<circuit::Node>& { return emptyNodes; });
    EXPECT_CALL(*mMockSchematicSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(false));

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
