/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/ConnectionSegmentation.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of ConnectionSegmentation.
 */
class ConnectionSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mConnectionSegmentation
            = std::make_unique<schematicSegmentation::ConnectionSegmentation>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets the behavior when finding contours.
     *
     * @param numberContours Number of contours found.
     */
    void onFindContours(const unsigned int numberContours)
    {
        ON_CALL(*mMockOpenCvWrapper, findContours)
            .WillByDefault([numberContours]([[maybe_unused]] ImageMat& image,
                                            Contours& contours,
                                            [[maybe_unused]] ContoursHierarchy& hierarchy,
                                            [[maybe_unused]] const OpenCvWrapper::RetrievalModes& mode,
                                            [[maybe_unused]] const OpenCvWrapper::ContourApproximationModes& method) {
                for (unsigned int i{0}; i < numberContours; ++i) {
                    Contour contour{};
                    contours.push_back(contour);
                }
            });
    }

    /**
     * @brief Expects calls to remove components when detecting connections.
     */
    void expectRemoveComponents()
    {
        const ImageMat image{};

        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).WillRepeatedly(Return(image));
        EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
    }

    /**
     * @brief Sets up detection of connections.
     *
     * @param connectionsDetected Number of connections detected.
     */
    void setupDetectConnections(const unsigned int connectionsDetected)
    {
        // Length is larger than the minimum to consider wire as a connection, if there are connections to detect
        const auto contLength{connectionsDetected > 0
                                  ? schematicSegmentation::ConnectionSegmentation::cConnectionMinLength
                                  : (schematicSegmentation::ConnectionSegmentation::cConnectionMinLength - 1)};

        // Setup expectations and behavior
        expectRemoveComponents();
        onFindContours(connectionsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(connectionsDetected).WillRepeatedly(Return(contLength));
    }

protected:
    /** Connection segmentation. */
    std::unique_ptr<schematicSegmentation::ConnectionSegmentation> mConnectionSegmentation;
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
 * @brief Tests that a single connection is detected.
 */
TEST_F(ConnectionSegmentationTest, detectsSingleConnection)
{
    constexpr auto expectedConnections{1};

    // Setup expectations and behavior
    setupDetectConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionSegmentation->detectConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionSegmentation->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that multiple connections are detected.
 */
TEST_F(ConnectionSegmentationTest, detectsMultipleConnections)
{
    constexpr auto expectedConnections{2};

    // Setup expectations and behavior
    setupDetectConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionSegmentation->detectConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionSegmentation->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected when there are no connections contours found.
 */
TEST_F(ConnectionSegmentationTest, detectsNoConnectionsWhenNoContours)
{
    constexpr auto expectedConnections{0};

    // Setup expectations and behavior
    setupDetectConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionSegmentation->detectConnections(image, image, components, false));

    // Number of connections detected
    const auto connectionsDetected{mConnectionSegmentation->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that no connections are detected when the area of the connection wire is small.
 */
TEST_F(ConnectionSegmentationTest, detectsNoConnectionsWhenSmallArea)
{
    // Length is smaller than the minimum to consider wire as a connection
    constexpr auto contLength{schematicSegmentation::ConnectionSegmentation::cConnectionMinLength - 1};
    constexpr auto connectionsContours{2};

    // Setup expectations and behavior
    expectRemoveComponents();
    onFindContours(connectionsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, arcLength).Times(connectionsContours).WillRepeatedly(Return(contLength));

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionSegmentation->detectConnections(image, image, components, false));

    // Number of connections detected
    constexpr auto expectedConnections{0};
    const auto connectionsDetected{mConnectionSegmentation->getDetectedConnections().size()};
    EXPECT_EQ(connectionsDetected, expectedConnections);
}

/**
 * @brief Tests that the image with detected connections is saved during processing, when connections are detected.
 */
TEST_F(ConnectionSegmentationTest, savesImagesWhenDetectsConnections)
{
    constexpr auto expectedConnections{1};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    setupDetectConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_TRUE(mConnectionSegmentation->detectConnections(image, image, components, saveImages));
}

/**
 * @brief Tests that the image with detected connections is not saved during processing, when connections are not
 * detected.
 */
TEST_F(ConnectionSegmentationTest, savesNoImagesWhenNoDetectedConnections)
{
    constexpr auto expectedConnections{0};
    constexpr auto saveImages{true};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    setupDetectConnections(expectedConnections);

    // Detect connections
    ImageMat image{};
    const std::vector<circuit::Component> components{circuit::Component{}};
    ASSERT_FALSE(mConnectionSegmentation->detectConnections(image, image, components, saveImages));
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
