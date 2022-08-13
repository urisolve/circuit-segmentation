/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/ConnectionDetection.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of ConnectionDetection.
 */
class ConnectionDetectionTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mConnectionDetection
            = std::make_unique<schematicSegmentation::ConnectionDetection>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets the behavior when finding contours.
     *
     * @param numContours Number of contours found.
     */
    void onFindContours(const unsigned int numContours)
    {
        ON_CALL(*mMockOpenCvWrapper, findContours)
            .WillByDefault([numContours]([[maybe_unused]] ImageMat& image,
                                         Contours& contours,
                                         [[maybe_unused]] ContoursHierarchy& hierarchy,
                                         [[maybe_unused]] const OpenCvWrapper::RetrievalModes& mode,
                                         [[maybe_unused]] const OpenCvWrapper::ContourApproximationModes& method) {
                for (unsigned int i{0}; i < numContours; ++i) {
                    Contour contour{};
                    contours.push_back(contour);
                }
            });
    }

    /**
     * @brief Expects calls to operations during generation of the image with only circuit connections.
     */
    void expectOperationsImageOnlyConnections()
    {
        const ImageMat img{};

        // Setup expectations and behavior
        EXPECT_CALL(*mMockOpenCvWrapper, getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, _))
            .Times(2)
            .WillRepeatedly(Return(img));
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_CLOSE, _, _)).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_OPEN, _, _)).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, bitwiseAnd).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).WillRepeatedly(Return(img));
    }

    /**
     * @brief Expects calls to generate bounding box for components to be removed.
     *
     * @param numComponents Number of components to be removed.
     */
    void expectRemoveBoundingBoxComponents(const unsigned int numComponents)
    {
        constexpr auto imgWidth{100};
        constexpr auto imgHeight{100};
        const Rectangle rect{};

        // Setup expectations and behavior
        ON_CALL(*mMockOpenCvWrapper, getImageWidth).WillByDefault(Return(imgWidth));
        ON_CALL(*mMockOpenCvWrapper, getImageHeight).WillByDefault(Return(imgHeight));
        ON_CALL(*mMockOpenCvWrapper, boundingRect).WillByDefault(Return(rect));
        EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(numComponents);
        EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(numComponents);
        EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(numComponents);
        EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(numComponents);
    }

    /**
     * @brief Expects calls to remove components.
     */
    void expectRemoveComponents()
    {
        const ImageMat image{};

        // Setup expectations and behavior
        EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).WillRepeatedly(Return(image));
        EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
    }

    /**
     * @brief Sets up detected connections.
     *
     * @param numConnectionsDetected Number of connections detected.
     */
    void setupDetectedConnections(const unsigned int numConnectionsDetected)
    {
        // Length is larger than the minimum to consider wire as a connection, if there are connections to detect
        const auto contLength{numConnectionsDetected > 0
                                  ? schematicSegmentation::ConnectionDetection::cConnectionMinLength
                                  : (schematicSegmentation::ConnectionDetection::cConnectionMinLength - 1)};

        // Setup expectations and behavior
        expectOperationsImageOnlyConnections();
        onFindContours(numConnectionsDetected);
        expectRemoveBoundingBoxComponents(numConnectionsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(2);
        ON_CALL(*mMockOpenCvWrapper, arcLength).WillByDefault(Return(contLength));
        EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(numConnectionsDetected);
    }

    /**
     * @brief Sets up detected connections during update.
     *
     * @param numConnectionsDetected Number of connections detected.
     */
    void setupDetectedConnectionsUpdate(const unsigned int numConnectionsDetected)
    {
        // Length is larger than the minimum to consider wire as a connection, if there are connections to detect
        const auto contLength{numConnectionsDetected > 0
                                  ? schematicSegmentation::ConnectionDetection::cConnectionMinLength
                                  : (schematicSegmentation::ConnectionDetection::cConnectionMinLength - 1)};

        // Setup expectations and behavior
        expectRemoveComponents();
        onFindContours(numConnectionsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
        ON_CALL(*mMockOpenCvWrapper, arcLength).WillByDefault(Return(contLength));
        EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(numConnectionsDetected);
    }

    /**
     * @brief Sets up a dummy connection.
     *
     * @param wire Connection wire.
     */
    void setupDummyConnection(const circuit::Wire& wire)
    {
        // Dummy connection
        circuit::Connection dummyConnection{};
        dummyConnection.mWire = wire;

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

protected:
    /** Connection detection. */
    std::unique_ptr<schematicSegmentation::ConnectionDetection> mConnectionDetection;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
};

/**
 * @brief Tests that a single connection is detected.
 */
TEST_F(ConnectionDetectionTest, detectsSingleConnection)
{
    constexpr auto expectedConnections{1};

    // Setup expectations and behavior
    setupDetectedConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    ASSERT_TRUE(mConnectionDetection->detectConnections(image, image, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that multiple connections are detected.
 */
TEST_F(ConnectionDetectionTest, detectsMultipleConnections)
{
    constexpr auto expectedConnections{2};

    // Setup expectations and behavior
    setupDetectedConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    ASSERT_TRUE(mConnectionDetection->detectConnections(image, image, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected when there are no connections contours found.
 */
TEST_F(ConnectionDetectionTest, detectsNoConnectionsWhenNoContours)
{
    constexpr auto expectedConnections{0};

    // Setup expectations and behavior
    setupDetectedConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    ASSERT_FALSE(mConnectionDetection->detectConnections(image, image, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected when the area of the connection wire is small.
 */
TEST_F(ConnectionDetectionTest, detectsNoConnectionsWhenSmallArea)
{
    // Length is smaller than the minimum to consider wire as a connection
    constexpr auto contLength{schematicSegmentation::ConnectionDetection::cConnectionMinLength - 1};
    constexpr auto connectionsContours{2};

    // Setup expectations and behavior
    expectOperationsImageOnlyConnections();
    onFindContours(connectionsContours);
    expectRemoveBoundingBoxComponents(connectionsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(connectionsContours).WillRepeatedly(Return(contLength));

    // Detect connections
    ImageMat image{};
    ASSERT_FALSE(mConnectionDetection->detectConnections(image, image, false));

    // Number of connections detected
    constexpr auto expectedConnections{0};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that the image with detected connections is saved during processing, when connections are detected.
 */
TEST_F(ConnectionDetectionTest, savesImagesWhenDetectsConnections)
{
    constexpr auto expectedConnections{1};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(5);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    setupDetectedConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    ASSERT_TRUE(mConnectionDetection->detectConnections(image, image, saveImages));
}

/**
 * @brief Tests that the image with detected connections is not saved during processing, when connections are not
 * detected.
 */
TEST_F(ConnectionDetectionTest, savesNoImagesWhenNoDetectedConnections)
{
    constexpr auto expectedConnections{0};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(4);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    setupDetectedConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    ASSERT_FALSE(mConnectionDetection->detectConnections(image, image, saveImages));
}

/**
 * @brief Tests that a single connection is detected during update.
 */
TEST_F(ConnectionDetectionTest, detectsSingleConnectionWhenUpdate)
{
    constexpr auto expectedConnections{1};

    // Setup expectations and behavior
    setupDetectedConnectionsUpdate(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->updateConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that multiple connections are detected during update.
 */
TEST_F(ConnectionDetectionTest, detectsMultipleConnectionsWhenUpdate)
{
    constexpr auto expectedConnections{2};

    // Setup expectations and behavior
    setupDetectedConnectionsUpdate(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->updateConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected during update when there are no connections contours found.
 */
TEST_F(ConnectionDetectionTest, detectsNoConnectionsWhenUpdateWhenNoContours)
{
    constexpr auto expectedConnections{0};

    // Setup expectations and behavior
    setupDetectedConnectionsUpdate(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionDetection->updateConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected during update when the area of the connection wire is small.
 */
TEST_F(ConnectionDetectionTest, detectsNoConnectionsWhenUpdateWhenSmallArea)
{
    // Length is smaller than the minimum to consider wire as a connection
    constexpr auto contLength{schematicSegmentation::ConnectionDetection::cConnectionMinLength - 1};
    constexpr auto connectionsContours{2};

    // Setup expectations and behavior
    expectRemoveComponents();
    onFindContours(connectionsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(connectionsContours).WillRepeatedly(Return(contLength));

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionDetection->updateConnections(image, image, components, false));

    // Number of connections detected
    constexpr auto expectedConnections{0};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that the image with detected connections during update is saved during processing, when connections are
 * detected.
 */
TEST_F(ConnectionDetectionTest, savesImagesWhenDetectsConnectionsWhenUpdate)
{
    constexpr auto expectedConnections{1};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    setupDetectedConnectionsUpdate(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->updateConnections(image, image, components, saveImages));
}

/**
 * @brief Tests that the image with detected connections during update is not saved during processing, when connections
 * are not detected.
 */
TEST_F(ConnectionDetectionTest, savesNoImagesWhenNoDetectedConnectionsWhenUpdate)
{
    constexpr auto expectedConnections{0};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    setupDetectedConnectionsUpdate(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionDetection->updateConnections(image, image, components, saveImages));
}

/**
 * @brief Tests that a single node is detected.
 *
 * Scenario:
 * - 3 components, A, B and C
 * - 1 connection with 3 components
 *
 * Expected:
 * - 1 node is created
 * - 3 connections are created
 */
TEST_F(ConnectionDetectionTest, detectsSingleNode)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(3).WillRepeatedly(Return(true));

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const computerVision::Point point3{15, 20};
    const circuit::Wire wire{point1, point2, point3};
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}, circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, false));

    // Number of nodes and connections detected
    const auto nodesDetected{mConnectionDetection->getDetectedNodes().size()};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    constexpr auto expectedNodes{1};
    constexpr auto expectedConnections{3};
    EXPECT_EQ(nodesDetected, expectedNodes);
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that multiple nodes are detected.
 *
 * Scenario:
 * - 3 components, A, B and C
 * - 2 connection with 3 components
 *
 * Expected:
 * - 2 node are created
 * - 6 connections are created
 */
TEST_F(ConnectionDetectionTest, detectsMultipleNodes)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(6).WillRepeatedly(Return(true));

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const computerVision::Point point3{15, 20};
    const circuit::Wire wire{point1, point2, point3};
    setupDummyConnection(wire);
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}, circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, false));

    // Number of nodes and connections detected
    const auto nodesDetected{mConnectionDetection->getDetectedNodes().size()};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    constexpr auto expectedNodes{2};
    constexpr auto expectedConnections{6};
    EXPECT_EQ(nodesDetected, expectedNodes);
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no nodes are detected when there are connections with less than 2 intersection points with
 * components.
 *
 * Scenario:
 * - 2 components, A and B
 * - 1 connection with 2 components
 *
 * Expected:
 * - 0 node are created
 * - 1 connection is created
 */
TEST_F(ConnectionDetectionTest, detectsNoNodesWhenInsufficientPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2).WillRepeatedly(Return(true));

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const circuit::Wire wire{point1, point2};
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, false));

    // Number of nodes and connections detected
    const auto nodesDetected{mConnectionDetection->getDetectedNodes().size()};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    constexpr auto expectedNodes{0};
    constexpr auto expectedConnections{1};
    EXPECT_EQ(nodesDetected, expectedNodes);
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no nodes are detected, as well as no connections, when there are no connections with
 intersection
 * points with components.
 *
 * Scenario:
 * - 3 components, A and B
 * - 2 connection with 0 components
 *
 * Expected:
 * - 0 node are created
 * - 0 connections are created
 */
TEST_F(ConnectionDetectionTest, detectsNoNodesNoConnectionsWhenNoPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2 * 3 * 3).WillRepeatedly(Return(false));

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const computerVision::Point point3{15, 20};
    const circuit::Wire wire{point1, point2, point3};
    setupDummyConnection(wire);
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}, circuit::Component{}};
    ASSERT_FALSE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, false));

    // Number of nodes and connections detected
    const auto nodesDetected{mConnectionDetection->getDetectedNodes().size()};
    const auto connectionsDetected{mConnectionDetection->getDetectedConnections().size()};
    constexpr auto expectedNodes{0};
    constexpr auto expectedConnections{0};
    EXPECT_EQ(nodesDetected, expectedNodes);
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that the image with detected connections and nodes is saved during processing, when connections and
 * nodes are detected.
 */
TEST_F(ConnectionDetectionTest, savesImagesWhenDetectsConnectionsAndNodes)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const computerVision::Point point3{15, 20};
    const circuit::Wire wire{point1, point2, point3};
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}, circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, saveImages));
}

/**
 * @brief Tests that the image with detected connections is saved during processing, when connections are detected
 but
 * no detected nodes.
 */
TEST_F(ConnectionDetectionTest, savesImagesWhenDetectsConnectionsButNoNodes)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);

    // Wire
    const computerVision::Point point1{10, 10};
    const computerVision::Point point2{20, 10};
    const circuit::Wire wire{point1, point2};
    setupDummyConnection(wire);
    mConnectionDetection->setDetectedConnections(mDummyConnections);

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}, circuit::Component{}};
    ASSERT_TRUE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, saveImages));
}

/**
 * @brief Tests that the image with detected connections and nodes is not saved during processing, when connections
 and
 * nodes are not detected.
 */
TEST_F(ConnectionDetectionTest, savesNoImagesWhenNoDetectedConnectionsAndNodes)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(0);

    // No connections
    mConnectionDetection->setDetectedConnections({});

    // Detect nodes and connections
    ImageMat image{};
    const std::vector<circuit::Component> components{};
    ASSERT_FALSE(mConnectionDetection->detectNodesUpdateConnections(image, image, components, saveImages));
}
