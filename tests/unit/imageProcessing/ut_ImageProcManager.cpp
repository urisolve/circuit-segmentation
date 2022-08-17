/**
 * @file
 */

#include "imageProcessing/ImageProcManager.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "mocks/imageProcessing/MockImagePreprocessing.h"
#include "mocks/imageProcessing/MockImageReceiver.h"
#include "mocks/imageProcessing/MockImageSegmentation.h"
#include "mocks/schematicSegmentation/MockRoiSegmentation.h"
#include "mocks/schematicSegmentation/MockSchematicSegmentation.h"
#include "mocks/schematicSegmentation/MockSegmentationMap.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;
using namespace circuitSegmentation::imageProcessing;
using namespace circuitSegmentation::schematicSegmentation;

/**
 * @brief Test class of ImageProcManager.
 */
class ImageProcManagerTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockImageReceiver = std::make_shared<NiceMock<MockImageReceiver>>(nullptr, nullptr);
        mMockImagePreprocessing = std::make_shared<NiceMock<MockImagePreprocessing>>(nullptr, nullptr);
        mMockImageSegmentation
            = std::make_shared<NiceMock<MockImageSegmentation>>(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        mMockSchematicSegmentation = std::make_shared<NiceMock<MockSchematicSegmentation>>(nullptr, nullptr);
        mMockRoiSegmentation = std::make_shared<NiceMock<MockRoiSegmentation>>(nullptr, nullptr);
        mMockSegmentationMap = std::make_shared<NiceMock<MockSegmentationMap>>(nullptr);
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        constexpr auto logMode{true};
        constexpr auto saveImages{false};

        // Setup expectations
        expectSetLogMode(logMode);
        expectSetSaveImages(saveImages);

        mImageProcManager = std::make_unique<ImageProcManager>(mMockImageReceiver,
                                                               mMockImagePreprocessing,
                                                               mMockImageSegmentation,
                                                               mMockSchematicSegmentation,
                                                               mMockRoiSegmentation,
                                                               mMockSegmentationMap,
                                                               mMockOpenCvWrapper,
                                                               mLogger,
                                                               logMode,
                                                               saveImages);

        onGetElements();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Expects call to set log mode.
     *
     * @param logMode Log mode.
     */
    void expectSetLogMode(const bool& logMode)
    {
        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, setLogMode(!logMode)).Times(1);
    }

    /**
     * @brief Expects call to save images.
     *
     * @param saveImages Save images.
     */
    void expectSetSaveImages(const bool& saveImages)
    {
        // Setup expectations
        EXPECT_CALL(*mMockImagePreprocessing, setSaveImages(saveImages)).Times(1);
        EXPECT_CALL(*mMockImageSegmentation, setSaveImages(saveImages)).Times(1);
    }

    /**
     * @brief Sets the behaviour when getting elements.
     */
    void onGetElements()
    {
        const std::vector<circuit::Component> emptyComponents{};
        const std::vector<circuit::Connection> emptyConnections{};
        const std::vector<circuit::Node> emptyNodes{};

        ON_CALL(*mMockSchematicSegmentation, getComponents)
            .WillByDefault(
                Invoke([&emptyComponents]() -> const std::vector<circuit::Component>& { return emptyComponents; }));
        ON_CALL(*mMockSchematicSegmentation, getConnections)
            .WillByDefault(
                Invoke([&emptyConnections]() -> const std::vector<circuit::Connection>& { return emptyConnections; }));
        ON_CALL(*mMockSchematicSegmentation, getNodes)
            .WillByDefault(Invoke([&emptyNodes]() -> const std::vector<circuit::Node>& { return emptyNodes; }));
    }

protected:
    /** Image processing manager. */
    std::unique_ptr<ImageProcManager> mImageProcManager;
    /** Image receiver. */
    std::shared_ptr<NiceMock<MockImageReceiver>> mMockImageReceiver;
    /** Image preprocessing. */
    std::shared_ptr<NiceMock<MockImagePreprocessing>> mMockImagePreprocessing;
    /** Image segmentation. */
    std::shared_ptr<NiceMock<MockImageSegmentation>> mMockImageSegmentation;
    /** Schematic segmentation. */
    std::shared_ptr<NiceMock<MockSchematicSegmentation>> mMockSchematicSegmentation;
    /** ROI segmentation. */
    std::shared_ptr<NiceMock<MockRoiSegmentation>> mMockRoiSegmentation;
    /** Segmentation map. */
    std::shared_ptr<NiceMock<MockSegmentationMap>> mMockSegmentationMap;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

/**
 * @brief Tests that the creation of a manager does not throw an exception.
 */
TEST_F(ImageProcManagerTest, createsManagerNoThrow)
{
    constexpr auto logMode{true};
    constexpr auto saveImages{true};

    // Create manager
    ASSERT_NO_THROW(ImageProcManager::create(mLogger, logMode, saveImages));
}

/**
 * @brief Tests that processing occurs successfully.
 */
TEST_F(ImageProcManagerTest, processesSuccessfully)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(3);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getConnections).Times(2);
    EXPECT_CALL(*mMockSchematicSegmentation, getNodes).Times(2);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiLabels).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSegmentationMap, generateSegmentationMap).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSegmentationMap, writeSegmentationMapJsonFile).Times(1).WillOnce(Return(true));

    // Process image
    const std::string imageFilePath{""};
    ASSERT_TRUE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when image reception failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenImageReceptionFailed)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when image segmentation failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenImageSegmentationFailed)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(0);

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when image generation with ROI for components failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenImageRoiComponentsFailed)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(1);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiComponents).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiLabels).Times(0);

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when image generation with ROI for labels failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenImageRoiLabelsFailed)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(2);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getConnections).Times(1);
    EXPECT_CALL(*mMockSchematicSegmentation, getNodes).Times(1);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiLabels).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockSegmentationMap, generateSegmentationMap).Times(0);

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when segmentation map generation failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenSegmentationMapFailed)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(3);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getConnections).Times(2);
    EXPECT_CALL(*mMockSchematicSegmentation, getNodes).Times(2);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiLabels).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSegmentationMap, generateSegmentationMap).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockSegmentationMap, writeSegmentationMapJsonFile).Times(0);

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that processing fails when writing segmentation map to a file failed.
 */
TEST_F(ImageProcManagerTest, processFailsWhenSegmentationMapWriteFailed)
{
    ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockImageReceiver, setImageFilePath).Times(1);
    EXPECT_CALL(*mMockImageReceiver, receiveImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockImageReceiver, getImageReceived).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockImagePreprocessing, preprocessImage).Times(1);
    EXPECT_CALL(*mMockImageSegmentation, segmentImage).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getComponents).Times(3);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiComponents).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSchematicSegmentation, getConnections).Times(2);
    EXPECT_CALL(*mMockSchematicSegmentation, getNodes).Times(2);
    EXPECT_CALL(*mMockRoiSegmentation, generateRoiLabels).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSegmentationMap, generateSegmentationMap).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockSegmentationMap, writeSegmentationMapJsonFile).Times(1).WillOnce(Return(false));

    // Process image
    const std::string imageFilePath{""};
    ASSERT_FALSE(mImageProcManager->processImage(imageFilePath));
}

/**
 * @brief Tests that the log mode is defined correctly.
 */
TEST_F(ImageProcManagerTest, setsLogMode)
{
    constexpr auto logMode{false};

    // Setup expectations
    expectSetLogMode(logMode);

    // Set log mode
    mImageProcManager->setLogMode(logMode);

    EXPECT_EQ(mImageProcManager->getLogMode(), logMode);
}

/**
 * @brief Tests that the flag to save images is defined correctly.
 */
TEST_F(ImageProcManagerTest, setsSaveImages)
{
    constexpr auto saveImages{true};

    // Setup expectations
    expectSetSaveImages(saveImages);

    // Set flag to save images
    mImageProcManager->setSaveImages(saveImages);

    EXPECT_EQ(mImageProcManager->getSaveImages(), saveImages);
}
