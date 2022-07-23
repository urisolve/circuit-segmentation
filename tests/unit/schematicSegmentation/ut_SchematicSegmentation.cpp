/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/SchematicSegmentation.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of SchematicSegmentation.
 */
class SchematicSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mSchematicSegmentation
            = std::make_unique<schematicSegmentation::SchematicSegmentation>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets up a dummy component.
     *
     * @param x X coordinate for component box.
     * @param y Y coordinate for component box.
     */
    void setupDummyComponent(const int& x, const int& y)
    {
        // Bounding box
        const Rectangle rectangle{x, y, cDimension, cDimension};

        // Dummy component
        circuit::Component dummyComponent{};
        dummyComponent.mBoundingBox = rectangle;

        // Set
        mDummyComponents.push_back(dummyComponent);
    }

    /**
     * @brief Sets up a dummy connection.
     *
     * @param x X coordinate for a point of connection wire.
     * @param y Y coordinate for a point of connection wire.
     */
    void setupDummyConnection(const int& x, const int& y)
    {
        // Wire
        const circuit::Wire wire{{x, y}};

        // Dummy connection
        circuit::Connection dummyConnection{};
        dummyConnection.mWire = wire;

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

    /**
     * @brief Sets the behavior when checking if a rectangle contains a given point.
     */
    void onCheckContainsPoint()
    {
        ON_CALL(*mMockOpenCvWrapper, contains).WillByDefault([](const Rectangle& rect, const Point& pt) {
            return (rect.x <= pt.x) && (pt.x < rect.x + rect.width) && (rect.y <= pt.y)
                   && (pt.y < rect.y + rect.height);
        });
    }

protected:
    /** Dimension for width and height for boxes of components to be used in tests. */
    static constexpr auto cDimension{10};

    /** Schematic segmentation. */
    std::unique_ptr<schematicSegmentation::SchematicSegmentation> mSchematicSegmentation;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy components to be used in tests. */
    std::vector<circuit::Component> mDummyComponents;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
};

/**
 * @brief Tests that a component connection point is detected when there is a component and a connection with
 * intersection points.
 *
 * Scenario:
 * - 1 component, A
 * - 1 connection with component A
 *
 * Expected:
 * - 1 connection point for component A
 * - Port of component A has the connection ID
 * - Connection start ID has the port ID
 */
TEST_F(SchematicSegmentationTest, detectsSingleComponentConnectionPoint)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);

    // Prepare circuit
    constexpr auto xCoordBox{5};
    constexpr auto yCoordBox{5};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension, yCoordBox + cDimension);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, false);

    // Check component connection points
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components.back().mPorts.size(), 1);

    // Check component port connection
    const auto connections{mSchematicSegmentation->getConnections()};
    const auto expectedPortConnectionId{connections.back().mId};
    EXPECT_EQ(components.back().mPorts.back().mConnectionId, expectedPortConnectionId);

    // Check connection start ID
    const auto expectedConnectionStartId{components.back().mPorts.back().mId};
    EXPECT_EQ(connections.back().mStartId, expectedConnectionStartId);
}

/**
 * @brief Tests that multiple component connections points are detected when there are components and connections with
 * intersection points.
 *
 * Scenario:
 * - 2 components, A and B
 * - 2 connections, C and D:
 *      - Connection C with components A and B
 *      - Connection D with component B only
 *
 * Expected:
 * - 1 connection point for component A
 * - 2 connection points for component B
 * - Port of component A has the connection C ID
 * - Port of component B has the connection C ID
 * - Other port of component B has the connection D ID
 * - Connection C start ID has the port ID of component A
 * - Connection C end ID has the port ID of component B
 * - Connection D start ID has the port ID of component B
 */
TEST_F(SchematicSegmentationTest, detectsMultipleComponentConnectionPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(4);

    // Prepare circuit
    constexpr auto xCoordBox1{5};
    constexpr auto yCoordBox1{5};
    constexpr auto xCoordBox2{16};
    constexpr auto yCoordBox2{16};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension, yCoordBox1 + cDimension);
    setupDummyConnection(xCoordBox2 + cDimension, yCoordBox2 + cDimension);

    // Detect component connections
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, false);

    // Check component connection points
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 2);
    const auto componentA{components.at(0)};
    const auto componentB{components.at(1)};
    EXPECT_EQ(componentA.mPorts.size(), 1);
    EXPECT_EQ(componentB.mPorts.size(), 2);

    // Check component port connection
    const auto connections{mSchematicSegmentation->getConnections()};
    const auto expectedPortAConnectionId{connections.at(0).mId};
    EXPECT_EQ(componentA.mPorts.back().mConnectionId, expectedPortAConnectionId);
    const auto expectedPort1BConnectionId{connections.at(0).mId};
    const auto expectedPort2BConnectionId{connections.at(1).mId};
    EXPECT_EQ(componentB.mPorts.at(0).mConnectionId, expectedPort1BConnectionId);
    EXPECT_EQ(componentB.mPorts.at(1).mConnectionId, expectedPort2BConnectionId);

    // Check connection start ID
    const auto expectedConnectionCStartId{componentA.mPorts.back().mId};
    const auto expectedConnectionCEndId{componentB.mPorts.at(0).mId};
    const auto expectedConnectionDStartId{componentB.mPorts.at(1).mId};
    EXPECT_EQ(connections.at(0).mStartId, expectedConnectionCStartId);
    EXPECT_EQ(connections.at(0).mEndId, expectedConnectionCEndId);
    EXPECT_EQ(connections.at(1).mStartId, expectedConnectionDStartId);
}

/**
 * @brief Tests that a component connection point is not detected when there is a component and a connection without
 * intersection points.
 *
 * Scenario:
 * - 1 component, A
 * - 1 connection without component A
 *
 * Expected:
 * - 0 connection points for component A
 * - Ports of component A are empty
 * - Connection start and end IDs are empty
 */
TEST_F(SchematicSegmentationTest, detectsNoComponentConnectionPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);

    // Prepare circuit
    constexpr auto xCoordBox{5};
    constexpr auto yCoordBox{5};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension + 1, yCoordBox + cDimension + 1);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, false);

    // Check component connection points
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components.back().mPorts.size(), 0);

    // Check connection start and end IDs
    const auto connections{mSchematicSegmentation->getConnections()};
    const auto expectedConnectionStartId{""};
    const auto expectedConnectionEndId{""};
    EXPECT_EQ(connections.back().mStartId, expectedConnectionStartId);
    EXPECT_EQ(connections.back().mEndId, expectedConnectionEndId);
}

/**
 * @brief Tests that a component connection point is not detected when there is a component but no connections detected.
 *
 * Scenario:
 * - 1 component, A
 * - 0 connections
 *
 * Expected:
 * - 0 connection points for component A
 * - Ports of component A are empty
 */
TEST_F(SchematicSegmentationTest, detectsNoComponentConnectionPointNoConnections)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Prepare circuit
    constexpr auto xCoordBox{5};
    constexpr auto yCoordBox{5};
    setupDummyComponent(xCoordBox, yCoordBox);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, {}, {}, false);

    // Check component connection points
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components.back().mPorts.size(), 0);
}

/**
 * @brief Tests that a component connection point is not detected when there are no components and no connections
 * detected.
 *
 * Scenario:
 * - 0 components
 * - 0 connections
 *
 * Expected:
 * - 0 components
 */
TEST_F(SchematicSegmentationTest, detectsNoComponentConnectionNoComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, {}, {}, {}, false);

    // Check component connection points
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 0);
}

/**
 * @brief Tests that the image with detected component ports is saved during processing, when at least a component
 * connection point is detected.
 */
TEST_F(SchematicSegmentationTest, savesImagesWhenDetectsComponentConnectionPoint)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);

    // Prepare circuit
    constexpr auto xCoordBox{5};
    constexpr auto yCoordBox{5};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension, yCoordBox + cDimension);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, saveImages);
}

/**
 * @brief Tests that the image with detected component ports is not saved during processing, when component connection
 * points are not detected.
 */
TEST_F(SchematicSegmentationTest, savesNoImagesWhenNoDetectedComponentConnectionPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(0);

    // Prepare circuit
    constexpr auto xCoordBox{5};
    constexpr auto yCoordBox{5};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension + 1, yCoordBox + cDimension + 1);

    // Detect component connection points
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, saveImages);
}

/**
 * @brief Tests that the list of detected components is kept when all the components have connection points.
 *
 * Scenario:
 * - 2 components, A and B
 * - 1 connection with components A and B
 *
 * Expected:
 * - List of components is kept
 */
TEST_F(SchematicSegmentationTest, updatesDetectedComponentsKept)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2);

    // Prepare circuit
    constexpr auto xCoordBox1{5};
    constexpr auto yCoordBox1{5};
    constexpr auto xCoordBox2{16};
    constexpr auto yCoordBox2{16};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension, yCoordBox1 + cDimension);

    // Detect component connections
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, false);

    // Check number of components
    const auto components{mSchematicSegmentation->getComponents()};
    constexpr auto expectedNumComponents{2};
    EXPECT_EQ(components.size(), expectedNumComponents);

    // Update detected components
    ASSERT_TRUE(mSchematicSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(detectedComponents.size(), expectedNumComponents);
}

/**
 * @brief Tests that the list of detected components is updated when there are components with no connections points.
 *
 * Scenario:
 * - 2 components, A and B
 * - 1 connection with component A
 * - 0 connections with component B
 *
 * Expected:
 * - List of components is updated
 */
TEST_F(SchematicSegmentationTest, updatesDetectedComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2);

    // Prepare circuit
    constexpr auto xCoordBox1{5};
    constexpr auto yCoordBox1{5};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension, yCoordBox1 + cDimension);

    // Detect component connections
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, mDummyConnections, {}, false);

    // Check number of components
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 2);

    // Update detected components
    ASSERT_TRUE(mSchematicSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
}

/**
 * @brief Tests that the list of detected components becomes empty when all components with no connection points.
 *
 * Scenario:
 * - 2 components, A and B
 * - 0 connections with component A
 * - 0 connections with component B
 *
 * Expected:
 * - List of components is empty
 */
TEST_F(SchematicSegmentationTest, updatesDetectedComponentsEmpty)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Prepare circuit
    constexpr auto xCoordBox1{5};
    constexpr auto yCoordBox1{5};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);

    // Detect component connections
    ImageMat img{};
    mSchematicSegmentation->detectComponentConnections(img, img, mDummyComponents, {}, {}, false);

    // Check number of components
    const auto components{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(components.size(), 2);

    // Update detected components
    ASSERT_FALSE(mSchematicSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mSchematicSegmentation->getComponents()};
    EXPECT_EQ(detectedComponents.size(), 0);
}

/**
 * @brief Tests that the relative position calculation for component port is done correctly when it is on box corners.
 */
TEST_F(SchematicSegmentationTest, calculatesPortPositionCorners)
{
    // Bounding box
    constexpr auto x{5};
    constexpr auto y{5};
    const Rectangle box{x, y, cDimension, cDimension};

    // Increments
    constexpr int widthIncr{2};
    constexpr int heightIncr{2};

    // Connection points
    const computerVision::Point pointTopLeft{box.x - widthIncr / 2, box.y - heightIncr / 2};
    const computerVision::Point pointBottomLeft{box.x - widthIncr / 2, box.y + box.height + heightIncr / 2};
    const computerVision::Point pointTopRight{box.x + box.width + widthIncr / 2, box.y - heightIncr / 2};
    const computerVision::Point pointBottomRight{box.x + box.width + widthIncr / 2,
                                                 box.y + box.height + heightIncr / 2};

    // Port position
    // Top left
    const auto positionTopLeft{mSchematicSegmentation->calcPortPosition(pointTopLeft, box, widthIncr, heightIncr)};
    constexpr auto expectedTopLeftX{0};
    constexpr auto expectedTopLeftY{0};
    EXPECT_DOUBLE_EQ(positionTopLeft.mX, expectedTopLeftX);
    EXPECT_DOUBLE_EQ(positionTopLeft.mY, expectedTopLeftX);
    // Bottom left
    const auto positionBottomLeft{
        mSchematicSegmentation->calcPortPosition(pointBottomLeft, box, widthIncr, heightIncr)};
    constexpr auto expectedBottomLeftX{0};
    constexpr auto expectedBottomLeftY{1};
    EXPECT_DOUBLE_EQ(positionBottomLeft.mX, expectedBottomLeftX);
    EXPECT_DOUBLE_EQ(positionBottomLeft.mY, expectedBottomLeftY);
    // Top right
    const auto positionTopRight{mSchematicSegmentation->calcPortPosition(pointTopRight, box, widthIncr, heightIncr)};
    constexpr auto expectedTopRightX{1};
    constexpr auto expectedTopRightY{0};
    EXPECT_DOUBLE_EQ(positionTopRight.mX, expectedTopRightX);
    EXPECT_DOUBLE_EQ(positionTopRight.mY, expectedTopRightY);
    // Bottom right
    const auto positionBottomRight{
        mSchematicSegmentation->calcPortPosition(pointBottomRight, box, widthIncr, heightIncr)};
    constexpr auto expectedBottomRightX{1};
    constexpr auto expectedBottomRightY{1};
    EXPECT_DOUBLE_EQ(positionBottomRight.mX, expectedBottomRightX);
    EXPECT_DOUBLE_EQ(positionBottomRight.mY, expectedBottomRightY);
}

/**
 * @brief Tests that the relative position calculation for component port is done correctly when it is on middle points
 * of box borders.
 */
TEST_F(SchematicSegmentationTest, calculatesPortPositionMiddle)
{
    // Bounding box
    constexpr auto x{50};
    constexpr auto y{50};
    constexpr auto dimension{20};
    const Rectangle box{x, y, dimension, dimension};

    // Increments
    constexpr int widthIncr{2};
    constexpr int heightIncr{2};

    // Connection points
    const computerVision::Point pointLeft50Perc{box.x - widthIncr / 2, box.y + box.height / 2};
    const computerVision::Point pointTop50Perc{box.x + box.width / 2, box.y - heightIncr / 2};
    const computerVision::Point pointRight75Perc{box.x + box.width + widthIncr / 2, box.y + box.height * 3 / 4};
    const computerVision::Point pointBottom25Perc{box.x + box.width / 4, box.y + box.height + heightIncr / 2};

    // Port position
    // Left 50%
    const auto positionLeft50Perc{
        mSchematicSegmentation->calcPortPosition(pointLeft50Perc, box, widthIncr, heightIncr)};
    constexpr auto expectedLeft50PercX{0};
    constexpr auto expectedLeft50PercY{0.5};
    EXPECT_DOUBLE_EQ(positionLeft50Perc.mX, expectedLeft50PercX);
    EXPECT_DOUBLE_EQ(positionLeft50Perc.mY, expectedLeft50PercY);
    // Top 50%
    const auto positionTop50Perc{mSchematicSegmentation->calcPortPosition(pointTop50Perc, box, widthIncr, heightIncr)};
    constexpr auto expectedTop50PercX{0.5};
    constexpr auto expectedTop50PercY{0};
    EXPECT_DOUBLE_EQ(positionTop50Perc.mX, expectedTop50PercX);
    EXPECT_DOUBLE_EQ(positionTop50Perc.mY, expectedTop50PercY);
    // Right 75%
    const auto positionRight75Perc{
        mSchematicSegmentation->calcPortPosition(pointRight75Perc, box, widthIncr, heightIncr)};
    constexpr auto expectedRight75PercX{1};
    constexpr auto expectedRight75PercY{0.75};
    EXPECT_DOUBLE_EQ(positionRight75Perc.mX, expectedRight75PercX);
    EXPECT_DOUBLE_EQ(positionRight75Perc.mY, expectedRight75PercY);
    // Bottom 25%
    const auto positionBottom25Perc{
        mSchematicSegmentation->calcPortPosition(pointBottom25Perc, box, widthIncr, heightIncr)};
    constexpr auto expectedBottom25PercX{0.25};
    constexpr auto expectedBottom25PercY{1};
    EXPECT_DOUBLE_EQ(positionBottom25Perc.mX, expectedBottom25PercX);
    EXPECT_DOUBLE_EQ(positionBottom25Perc.mY, expectedBottom25PercY);
}
