/**
 * @file
 */

#include "imageProcessing/ImageSegmentation.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "mocks/schematicSegmentation/MockComponentDetection.h"
#include "mocks/schematicSegmentation/MockConnectionDetection.h"
#include "mocks/schematicSegmentation/MockLabelDetection.h"
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
        mMockLabelDetection = std::make_shared<NiceMock<MockLabelDetection>>(nullptr, nullptr);
        mMockSchematicSegmentation = std::make_shared<NiceMock<MockSchematicSegmentation>>(nullptr, nullptr);

        mImageSegmentation = std::make_unique<imageProcessing::ImageSegmentation>(mMockOpenCvWrapper,
                                                                                  mLogger,
                                                                                  mMockComponentDetection,
                                                                                  mMockConnectionDetection,
                                                                                  mMockLabelDetection,
                                                                                  mMockSchematicSegmentation,
                                                                                  false);

        onGetElements();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets the behaviour when getting elements.
     */
    void onGetElements()
    {
        const std::vector<circuit::Component> emptyComponents{};
        const std::vector<circuit::Connection> emptyConnections{};
        const std::vector<circuit::Node> emptyNodes{};
        const std::vector<circuit::Label> emptyLabels{};

        ON_CALL(*mMockComponentDetection, getDetectedComponents)
            .WillByDefault([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
        ON_CALL(*mMockConnectionDetection, getDetectedConnections)
            .WillByDefault(
                [&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
        ON_CALL(*mMockConnectionDetection, getDetectedNodes)
            .WillByDefault([&emptyNodes]() -> const std::vector<circuit::Node>& { return emptyNodes; });
        ON_CALL(*mMockSchematicSegmentation, getComponents)
            .WillByDefault([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; });
        ON_CALL(*mMockSchematicSegmentation, getConnections)
            .WillByDefault(
                [&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; });
        ON_CALL(*mMockLabelDetection, getDetectedLabels)
            .WillByDefault([&emptyLabels]() -> const std::vector<circuit::Label>& { return emptyLabels; });
    }

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
    /** Label detection. */
    std::shared_ptr<NiceMock<MockLabelDetection>> mMockLabelDetection;
    /** Schematic segmentation. */
    std::shared_ptr<NiceMock<MockSchematicSegmentation>> mMockSchematicSegmentation;
};

/**
 * @brief Tests that segmentation occurs successfully.
 */
TEST_F(ImageSegmentationTest, segmentsSuccessfully)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockLabelDetection, detectLabels).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, associateLabels).Times(1);

    EXPECT_CALL(*mMockConnectionDetection, getDetectedConnections).Times(2);
    EXPECT_CALL(*mMockComponentDetection, getDetectedComponents).Times(3);
    EXPECT_CALL(*mMockConnectionDetection, getDetectedNodes).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, getConnections).Times(1);
    EXPECT_CALL(*mMockLabelDetection, getDetectedLabels).Times(1);

    // Segment image
    ImageMat image{};
    ASSERT_TRUE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation occurs successfully when there are no labels detected.
 */
TEST_F(ImageSegmentationTest, segmentsSuccessfullyWhenNoLabels)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, detectComponentConnections).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, updateDetectedComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockLabelDetection, detectLabels).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockSchematicSegmentation, associateLabels).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_TRUE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no connections detected.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoConnections)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no components detected.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoComponents)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(0);

    // Segment image
    ImageMat image{};
    ASSERT_FALSE(mImageSegmentation->segmentImage(image, image));
}

/**
 * @brief Tests that segmentation fails when there are no connections detected after update.
 */
TEST_F(ImageSegmentationTest, segmentFailsWhenNoUpdatedConnections)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(1).WillOnce(Return(false));
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
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(1).WillOnce(Return(true));
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
    // Setup expectations and behavior
    EXPECT_CALL(*mMockConnectionDetection, detectConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockComponentDetection, detectComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, updateConnections).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockConnectionDetection, detectNodesUpdateConnections).Times(1).WillOnce(Return(true));
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
