/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/RoiSegmentation.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of RoiSegmentation.
 */
class RoiSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mRoiSegmentation = std::make_unique<schematicSegmentation::RoiSegmentation>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets up a dummy component.
     *
     * @param numLabels Number of labels associated to this component.
     */
    void setupDummyComponent(const unsigned char& numLabels)
    {
        // Bounding box
        constexpr auto x{0};
        constexpr auto y{0};
        constexpr auto dimension{10};
        const Rectangle rectangle{x, y, dimension, dimension};

        // Associated labels
        std::vector<circuit::Label> labels{};
        for (auto i{0}; i < numLabels; ++i) {
            // Bounding box for label
            circuit::Label dummyLabel{};
            dummyLabel.mBoundingBox = rectangle;
            labels.push_back(dummyLabel);
        }

        // Dummy component
        circuit::Component dummyComponent{};
        dummyComponent.mBoundingBox = rectangle;
        dummyComponent.mLabels = labels;

        // Set
        mDummyComponents.push_back(dummyComponent);
    }

    /**
     * @brief Sets up a dummy connection.
     *
     * @param numLabels Number of labels associated to this component.
     */
    void setupDummyConnection(const unsigned char& numLabels)
    {
        // Bounding box
        constexpr auto x{0};
        constexpr auto y{0};
        constexpr auto dimension{10};
        const Rectangle rectangle{x, y, dimension, dimension};

        // Associated labels
        std::vector<circuit::Label> labels{};
        for (auto i{0}; i < numLabels; ++i) {
            // Bounding box for label
            circuit::Label dummyLabel{};
            dummyLabel.mBoundingBox = rectangle;
            labels.push_back(dummyLabel);
        }

        // Dummy connection
        circuit::Connection dummyConnection{};
        dummyConnection.mLabels = labels;

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

    /**
     * @brief Sets up a dummy node.
     *
     * @param numLabels Number of labels associated to this component.
     */
    void setupDummyNode(const unsigned char& numLabels)
    {
        // Bounding box
        constexpr auto x{0};
        constexpr auto y{0};
        constexpr auto dimension{10};
        const Rectangle rectangle{x, y, dimension, dimension};

        // Associated labels
        std::vector<circuit::Label> labels{};
        for (auto i{0}; i < numLabels; ++i) {
            // Bounding box for label
            circuit::Label dummyLabel{};
            dummyLabel.mBoundingBox = rectangle;
            labels.push_back(dummyLabel);
        }

        // Dummy node
        circuit::Node dummyNode{};
        dummyNode.mLabels = labels;

        // Set
        mDummyNodes.push_back(dummyNode);
    }

protected:
    /** ROI segmentation. */
    std::unique_ptr<schematicSegmentation::RoiSegmentation> mRoiSegmentation;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy components to be used in tests. */
    std::vector<circuit::Component> mDummyComponents;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
    /** Dummy nodes to be used in tests. */
    std::vector<circuit::Node> mDummyNodes;
};

/**
 * @brief Tests that generation of images with ROI reports success when they are successfully generated for all
 * components.
 *
 * Scenario:
 * - 2 components are passed to generate images with ROI
 * - Image is successfully cropped and written for all components
 *
 * Expected:
 * - Generation reports success
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsSuccessOnGenerationRoiComponents)
{
    // Setup components
    setupDummyComponent(0);
    setupDummyComponent(0);
    const std::string expectFilePath1{"roi_component_" + mDummyComponents.at(0).mId + ".png"};
    const std::string expectFilePath2{"roi_component_" + mDummyComponents.at(1).mId + ".png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for components
    ImageMat img{};
    ASSERT_TRUE(mRoiSegmentation->generateRoiComponents(img, mDummyComponents));
}

/**
 * @brief Tests that generation of images with ROI reports unsuccess when at least one image is unsuccessfully
 * cropped.
 *
 * Scenario:
 * - 2 components are passed to generate images with ROI
 * - Image is successfully cropped and written for the second component
 * - Image is unsuccessfully cropped for the first component
 *
 * Expected:
 * - Generation method returns unsuccess
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsUnsuccessOnGenerationRoiComponentsWhenCrop)
{
    // Setup components
    setupDummyComponent(0);
    setupDummyComponent(0);
    const std::string expectFilePath1{"roi_component_" + mDummyComponents.at(0).mId + ".png"};
    const std::string expectFilePath2{"roi_component_" + mDummyComponents.at(1).mId + ".png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(2).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath1, _)).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for components
    ImageMat img{};
    ASSERT_FALSE(mRoiSegmentation->generateRoiComponents(img, mDummyComponents));
}

/**
 * @brief Tests that generation of images with ROI reports unsuccess when at least one image is unsuccessfully
 * written.
 *
 * Scenario:
 * - 3 components are passed to generate images with ROI
 * - Image is successfully cropped and written for the first and third component
 * - Image is successfully cropped but unsuccessfully written for the second component
 *
 * Expected:
 * - Generation method returns unsuccess
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsUnsuccessOnGenerationRoiComponentsWhenWrite)
{
    // Setup components
    setupDummyComponent(0);
    setupDummyComponent(0);
    setupDummyComponent(0);
    const std::string expectFilePath1{"roi_component_" + mDummyComponents.at(0).mId + ".png"};
    const std::string expectFilePath2{"roi_component_" + mDummyComponents.at(1).mId + ".png"};
    const std::string expectFilePath3{"roi_component_" + mDummyComponents.at(2).mId + ".png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath2, _)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePath3, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for components
    ImageMat img{};
    ASSERT_FALSE(mRoiSegmentation->generateRoiComponents(img, mDummyComponents));
}

/**
 * @brief Tests that generation of images with ROI reports success when they are successfully generated for all
 * labels.
 *
 * Scenario:
 * - 2 components with 1 label associated
 * - 2 connections with 1 label associated
 * - 2 nodes with 1 label associated
 * - Image is successfully cropped and written for all components, connections and nodes
 *
 * Expected:
 * - Generation reports success
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsSuccessOnGenerationRoiLabels)
{
    // Setup labels
    setupDummyComponent(1);
    setupDummyComponent(1);
    setupDummyConnection(1);
    setupDummyConnection(1);
    setupDummyNode(1);
    setupDummyNode(1);
    const std::string expectFilePathComponent1{"roi_label_" + mDummyComponents.at(0).mId + "_1.png"};
    const std::string expectFilePathComponent2{"roi_label_" + mDummyComponents.at(1).mId + "_1.png"};
    const std::string expectFilePathConnection1{"roi_label_" + mDummyConnections.at(0).mId + "_1.png"};
    const std::string expectFilePathConnection2{"roi_label_" + mDummyConnections.at(1).mId + "_1.png"};
    const std::string expectFilePathNode1{"roi_label_" + mDummyNodes.at(0).mId + "_1.png"};
    const std::string expectFilePathNode2{"roi_label_" + mDummyNodes.at(1).mId + "_1.png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(6).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for labels
    ImageMat img{};
    ASSERT_TRUE(mRoiSegmentation->generateRoiLabels(img, mDummyComponents, mDummyConnections, mDummyNodes));
}

/**
 * @brief Tests that generation of images with ROI reports success when they are successfully generated for all
 * labels, using multiple labels for elements.
 *
 * Scenario:
 * - 1 component with 2 labels associated
 * - 1 connection with 2 labels associated
 * - 1 node with 2 labels associated
 * - Image is successfully cropped and written for all components, connections and nodes
 *
 * Expected:
 * - Generation reports success
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsSuccessOnGenerationRoiLabelsMultiple)
{
    // Setup labels
    setupDummyComponent(2);
    setupDummyConnection(2);
    setupDummyNode(2);
    const std::string expectFilePathComponentLabel1{"roi_label_" + mDummyComponents.back().mId + "_1.png"};
    const std::string expectFilePathComponentLabel2{"roi_label_" + mDummyComponents.back().mId + "_2.png"};
    const std::string expectFilePathConnectionLabel1{"roi_label_" + mDummyConnections.back().mId + "_1.png"};
    const std::string expectFilePathConnectionLabel2{"roi_label_" + mDummyConnections.back().mId + "_2.png"};
    const std::string expectFilePathNodeLabel1{"roi_label_" + mDummyNodes.back().mId + "_1.png"};
    const std::string expectFilePathNodeLabel2{"roi_label_" + mDummyNodes.back().mId + "_2.png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(6).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponentLabel1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponentLabel2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnectionLabel1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnectionLabel2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNodeLabel1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNodeLabel2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for labels
    ImageMat img{};
    ASSERT_TRUE(mRoiSegmentation->generateRoiLabels(img, mDummyComponents, mDummyConnections, mDummyNodes));
}

/**
 * @brief Tests that generation of images with ROI reports unsuccess when at least one image is unsuccessfully
 * cropped.
 *
 * Scenario:
 * - 2 components with 1 label associated
 * - 2 connections with 1 label associated
 * - 2 nodes with 1 label associated
 * - Image is successfully cropped and written for the first component and for all connections and nodes
 * - Image is unsuccessfully cropped for the second component
 *
 * Expected:
 * - Generation reports unsuccess
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsUnsuccessOnGenerationRoiLabelsWhenCrop)
{
    // Setup labels
    setupDummyComponent(1);
    setupDummyComponent(1);
    setupDummyConnection(1);
    setupDummyConnection(1);
    setupDummyNode(1);
    setupDummyNode(1);
    const std::string expectFilePathComponent1{"roi_label_" + mDummyComponents.at(0).mId + "_1.png"};
    const std::string expectFilePathComponent2{"roi_label_" + mDummyComponents.at(1).mId + "_1.png"};
    const std::string expectFilePathConnection1{"roi_label_" + mDummyConnections.at(0).mId + "_1.png"};
    const std::string expectFilePathConnection2{"roi_label_" + mDummyConnections.at(1).mId + "_1.png"};
    const std::string expectFilePathNode1{"roi_label_" + mDummyNodes.at(0).mId + "_1.png"};
    const std::string expectFilePathNode2{"roi_label_" + mDummyNodes.at(1).mId + "_1.png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage)
        .Times(6)
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent2, _)).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for labels
    ImageMat img{};
    ASSERT_FALSE(mRoiSegmentation->generateRoiLabels(img, mDummyComponents, mDummyConnections, mDummyNodes));
}

/**
 * @brief Tests that generation of images with ROI reports unsuccess when at least one image is unsuccessfully
 * written.
 *
 * Scenario:
 * - 2 components with 1 label associated
 * - 2 connections with 1 label associated
 * - 2 nodes with 1 label associated
 * - Image is successfully cropped and written for all components, connections and for the second node
 * - Image is successfully cropped but unsuccessfully written for the first node
 *
 * Expected:
 * - Generation reports unsuccess
 * - Files with images have correct name
 */
TEST_F(RoiSegmentationTest, reportsUnsuccessOnGenerationRoiLabelsWhenWrite)
{
    // Setup labels
    setupDummyComponent(1);
    setupDummyComponent(1);
    setupDummyConnection(1);
    setupDummyConnection(1);
    setupDummyNode(1);
    setupDummyNode(1);
    const std::string expectFilePathComponent1{"roi_label_" + mDummyComponents.at(0).mId + "_1.png"};
    const std::string expectFilePathComponent2{"roi_label_" + mDummyComponents.at(1).mId + "_1.png"};
    const std::string expectFilePathConnection1{"roi_label_" + mDummyConnections.at(0).mId + "_1.png"};
    const std::string expectFilePathConnection2{"roi_label_" + mDummyConnections.at(1).mId + "_1.png"};
    const std::string expectFilePathNode1{"roi_label_" + mDummyNodes.at(0).mId + "_1.png"};
    const std::string expectFilePathNode2{"roi_label_" + mDummyNodes.at(1).mId + "_1.png"};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, cropImage).Times(6).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathComponent2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection1, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathConnection2, _)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode1, _)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage(expectFilePathNode2, _)).Times(1).WillOnce(Return(true));

    // Generate ROI for labels
    ImageMat img{};
    ASSERT_FALSE(mRoiSegmentation->generateRoiLabels(img, mDummyComponents, mDummyConnections, mDummyNodes));
}
