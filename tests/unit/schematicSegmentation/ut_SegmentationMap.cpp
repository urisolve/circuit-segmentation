/**
 * @file
 */

#include "logging/Logger.h"
#include "schematicSegmentation/SegmentationMap.h"
#include "schematicSegmentation/SegmentationUtils.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of SegmentationMap.
 */
class SegmentationMapTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mSegmentationMap = std::make_unique<schematicSegmentation::SegmentationMap>(mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Generates a dummy label.
     *
     * @param ownerId Label owner ID.
     *
     * @return Dummy label.
     */
    circuit::Label generateDummyLabel(const circuit::Id& ownerId)
    {
        // Dummy label
        circuit::Label dummyLabel{};
        dummyLabel.mOwnerId = ownerId;

        return dummyLabel;
    }

    /**
     * @brief Generates a dummy port.
     *
     * @param ownerId Port owner ID.
     * @param connectionId Port connection ID.
     *
     * @return Dummy port.
     */
    circuit::Port generateDummyPort(const circuit::Id& ownerId, const circuit::Id& connectionId)
    {
        // Dummy port
        circuit::Port dummyPort{};
        dummyPort.mOwnerId = ownerId;
        dummyPort.mConnectionId = connectionId;

        return dummyPort;
    }

    /**
     * @brief Sets up a dummy component.
     *
     * @param numPorts Number of component ports.
     */
    void setupDummyComponent(const unsigned int& numPorts)
    {
        // Dummy component
        circuit::Component dummyComponent{};
        // Label
        dummyComponent.mLabel = generateDummyLabel(dummyComponent.mId);

        // Ports
        for (auto i{0}; i < numPorts; ++i) {
            circuit::Connection dummyConnection{};
            dummyComponent.mPorts.push_back(generateDummyPort(dummyComponent.mId, dummyConnection.mId));
        }

        // Position
        dummyComponent.mPosition.mX = 100;
        dummyComponent.mPosition.mY = 100;
        dummyComponent.mPosition.mAngle = 0;

        // Set
        mDummyComponents.push_back(dummyComponent);
    }

    /**
     * @brief Sets up a dummy connection.
     *
     * @param startId Connection start ID.
     * @param endId Connection end ID.
     */
    void setupDummyConnection(const circuit::Id& startId, const circuit::Id& endId)
    {
        // Dummy connection
        circuit::Connection dummyConnection{};
        // Start ID
        dummyConnection.mStartId = startId;
        // End ID
        dummyConnection.mEndId = endId;
        // Label
        dummyConnection.mLabel = generateDummyLabel(dummyConnection.mId);

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

    /**
     * @brief Sets up a dummy node.
     *
     * @param numConnectionsIds Number of node connections IDs.
     */
    void setupDummyNode(const unsigned int& numConnectionsIds)
    {
        // Dummy node
        circuit::Node dummyNode{};
        // Label
        dummyNode.mLabel = generateDummyLabel(dummyNode.mId);
        // Connections IDs
        for (auto i{0}; i < numConnectionsIds; ++i) {
            circuit::Connection dummyConnection{};
            dummyNode.mConnectionIds.push_back(dummyConnection.mId);
        }

        // Set
        mDummyNodes.push_back(dummyNode);
    }

    /**
     * @brief Compares JSON component with component.
     *
     * @param jsonComponent JSON component.
     * @param component Component.
     */
    void compareJsonComponent(const nlohmann::ordered_json& jsonComponent, const circuit::Component& component)
    {
        // ID
        EXPECT_EQ(jsonComponent["id"], component.mId);
        // Type
        EXPECT_EQ(jsonComponent["type"], component.mType);
        // Full name
        EXPECT_EQ(jsonComponent["fullName"], component.mFullName);
        // Label
        EXPECT_EQ(jsonComponent["label"]["id"], component.mLabel.mId);
        EXPECT_EQ(jsonComponent["label"]["owner"], component.mLabel.mOwnerId);
        EXPECT_EQ(jsonComponent["label"]["name"], component.mLabel.mName);
        EXPECT_EQ(jsonComponent["label"]["value"], component.mLabel.mValue);
        EXPECT_EQ(jsonComponent["label"]["unit"], component.mLabel.mUnit);
        EXPECT_EQ(jsonComponent["label"]["position"]["x"], component.mLabel.mPosition.mX);
        EXPECT_EQ(jsonComponent["label"]["position"]["y"], component.mLabel.mPosition.mY);
        EXPECT_EQ(jsonComponent["label"]["position"]["angle"], component.mLabel.mPosition.mAngle);
        EXPECT_EQ(jsonComponent["label"]["isNameHidden"], component.mLabel.mIsNameHidden);
        EXPECT_EQ(jsonComponent["label"]["isValueHidden"], component.mLabel.mIsValueHidden);

        // Ports
        for (auto it{jsonComponent["ports"].begin()}; it != jsonComponent["ports"].end(); ++it) {
            const auto index{it - jsonComponent["ports"].begin()};

            // Port
            const auto jsonPort{jsonComponent["ports"].at(index)};
            const auto port{component.mPorts.at(index)};

            EXPECT_EQ(jsonPort["id"], port.mId);
            EXPECT_EQ(jsonPort["owner"], port.mOwnerId);
            EXPECT_EQ(jsonPort["type"], port.mType);
            EXPECT_DOUBLE_EQ(jsonPort["position"]["x"], schematicSegmentation::roundDouble(port.mPosition.mX, 1));
            EXPECT_DOUBLE_EQ(jsonPort["position"]["y"], schematicSegmentation::roundDouble(port.mPosition.mY, 1));
            EXPECT_EQ(jsonPort["position"]["angle"], port.mPosition.mAngle);
            EXPECT_EQ(jsonPort["connection"], port.mConnectionId);
        }

        // Position
        EXPECT_EQ(jsonComponent["position"]["x"], component.mPosition.mX);
        EXPECT_EQ(jsonComponent["position"]["y"], component.mPosition.mY);
        EXPECT_EQ(jsonComponent["position"]["angle"], component.mPosition.mAngle);
    }

    /**
     * @brief Compares JSON connection with connection.
     *
     * @param jsonConnection JSON connection.
     * @param connection Connection.
     */
    void compareJsonConnection(const nlohmann::ordered_json& jsonConnection, const circuit::Connection& connection)
    {
        // ID
        EXPECT_EQ(jsonConnection["id"], connection.mId);
        // Start ID
        EXPECT_EQ(jsonConnection["start"], connection.mStartId);
        // End ID
        EXPECT_EQ(jsonConnection["end"], connection.mEndId);
        // Label
        EXPECT_EQ(jsonConnection["label"]["id"], connection.mLabel.mId);
        EXPECT_EQ(jsonConnection["label"]["owner"], connection.mLabel.mOwnerId);
        EXPECT_EQ(jsonConnection["label"]["name"], connection.mLabel.mName);
        EXPECT_EQ(jsonConnection["label"]["value"], connection.mLabel.mValue);
        EXPECT_EQ(jsonConnection["label"]["unit"], connection.mLabel.mUnit);
        EXPECT_EQ(jsonConnection["label"]["position"]["x"], connection.mLabel.mPosition.mX);
        EXPECT_EQ(jsonConnection["label"]["position"]["y"], connection.mLabel.mPosition.mY);
        EXPECT_EQ(jsonConnection["label"]["position"]["angle"], connection.mLabel.mPosition.mAngle);
        EXPECT_EQ(jsonConnection["label"]["isNameHidden"], connection.mLabel.mIsNameHidden);
        EXPECT_EQ(jsonConnection["label"]["isValueHidden"], connection.mLabel.mIsValueHidden);
    }

    /**
     * @brief Compares JSON node with node.
     *
     * @param jsonNode JSON node.
     * @param node Node.
     */
    void compareJsonNode(const nlohmann::ordered_json& jsonNode, const circuit::Node& node)
    {
        // ID
        EXPECT_EQ(jsonNode["id"], node.mId);
        // Label
        EXPECT_EQ(jsonNode["label"]["id"], node.mLabel.mId);
        EXPECT_EQ(jsonNode["label"]["owner"], node.mLabel.mOwnerId);
        EXPECT_EQ(jsonNode["label"]["name"], node.mLabel.mName);
        EXPECT_EQ(jsonNode["label"]["value"], node.mLabel.mValue);
        EXPECT_EQ(jsonNode["label"]["unit"], node.mLabel.mUnit);
        EXPECT_EQ(jsonNode["label"]["position"]["x"], node.mLabel.mPosition.mX);
        EXPECT_EQ(jsonNode["label"]["position"]["y"], node.mLabel.mPosition.mY);
        EXPECT_EQ(jsonNode["label"]["position"]["angle"], node.mLabel.mPosition.mAngle);
        EXPECT_EQ(jsonNode["label"]["isNameHidden"], node.mLabel.mIsNameHidden);
        EXPECT_EQ(jsonNode["label"]["isValueHidden"], node.mLabel.mIsValueHidden);
        // Position
        EXPECT_EQ(jsonNode["position"]["x"], node.mPosition.mX);
        EXPECT_EQ(jsonNode["position"]["y"], node.mPosition.mY);
        EXPECT_EQ(jsonNode["position"]["angle"], node.mPosition.mAngle);

        // Connections IDs
        for (auto it{jsonNode["connections"].begin()}; it != jsonNode["connections"].end(); ++it) {
            const auto index{it - jsonNode["connections"].begin()};

            // Connection ID
            EXPECT_EQ(jsonNode["connections"].at(index), node.mConnectionIds.at(index));
        }

        // Type
        EXPECT_EQ(jsonNode["type"], node.mType);
    }

protected:
    /** Segmentation map. */
    std::unique_ptr<schematicSegmentation::SegmentationMap> mSegmentationMap;
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
 * @brief Tests that the segmentation map is generated successfully.
 *
 * Scenario:
 * - 2 component
 * - 2 connections
 * - 2 nodes
 *
 * Expected:
 * - Segmentation map generated correctly
 */
TEST_F(SegmentationMapTest, generatesMapSuccessfully)
{
    // Prepare circuit
    constexpr auto numPorts1{1};
    constexpr auto numPorts2{2};
    setupDummyComponent(numPorts1);
    setupDummyComponent(numPorts2);
    constexpr auto numConnectionsIds1{3};
    constexpr auto numConnectionsIds2{4};
    setupDummyNode(numConnectionsIds1);
    setupDummyNode(numConnectionsIds2);
    const auto startId1{mDummyComponents.at(0).mPorts.back().mId};
    const auto endId1{mDummyNodes.at(0).mId};
    const auto startId2{mDummyComponents.at(1).mPorts.at(0).mId};
    const auto endId2{mDummyNodes.at(1).mId};
    setupDummyConnection(startId1, endId1);
    setupDummyConnection(startId2, endId2);

    // Generate map
    ASSERT_TRUE(mSegmentationMap->generateSegmentationMap(mDummyComponents, mDummyConnections, mDummyNodes));

    // Get generated map
    const auto map{mSegmentationMap->getSegmentationMap()};

    // Check components in map
    const auto expectNumComponents{mDummyComponents.size()};
    EXPECT_EQ(map["components"].size(), expectNumComponents);
    for (auto it{map["components"].begin()}; it != map["components"].end(); ++it) {
        const auto index{it - map["components"].begin()};
        compareJsonComponent(map["components"].at(index), mDummyComponents.at(index));
    }

    // Check nodes in map
    const auto expectNumNodes{mDummyNodes.size()};
    EXPECT_EQ(map["nodes"].size(), expectNumNodes);
    for (auto it{map["nodes"].begin()}; it != map["nodes"].end(); ++it) {
        const auto index{it - map["nodes"].begin()};
        compareJsonNode(map["nodes"].at(index), mDummyNodes.at(index));
    }

    // Check connections in map
    const auto expectNumConnections{mDummyConnections.size()};
    EXPECT_EQ(map["connections"].size(), expectNumConnections);
    for (auto it{map["connections"].begin()}; it != map["connections"].end(); ++it) {
        const auto index{it - map["connections"].begin()};
        compareJsonConnection(map["connections"].at(index), mDummyConnections.at(index));
    }
}

/**
 * @brief Tests that the segmentation map is written successfully.
 *
 * Scenario:
 * - 2 component
 * - 2 connections
 * - 2 nodes
 *
 * Expected:
 * - Segmentation map is written
 */
TEST_F(SegmentationMapTest, writesMapSuccessfully)
{
    // Prepare circuit
    constexpr auto numPorts1{1};
    constexpr auto numPorts2{2};
    setupDummyComponent(numPorts1);
    setupDummyComponent(numPorts2);
    constexpr auto numConnectionsIds1{3};
    constexpr auto numConnectionsIds2{4};
    setupDummyNode(numConnectionsIds1);
    setupDummyNode(numConnectionsIds2);
    const auto startId1{mDummyComponents.at(0).mPorts.back().mId};
    const auto endId1{mDummyNodes.at(0).mId};
    const auto startId2{mDummyComponents.at(1).mPorts.at(0).mId};
    const auto endId2{mDummyNodes.at(1).mId};
    setupDummyConnection(startId1, endId1);
    setupDummyConnection(startId2, endId2);

    // Generate map
    ASSERT_TRUE(mSegmentationMap->generateSegmentationMap(mDummyComponents, mDummyConnections, mDummyNodes));

    // Write map
    ASSERT_TRUE(mSegmentationMap->writeSegmentationMapJsonFile());
}

/**
 * @brief Tests that the segmentation map is written when the map is empty.
 */
TEST_F(SegmentationMapTest, writesEmptyMap)
{
    // Write map
    ASSERT_TRUE(mSegmentationMap->writeSegmentationMapJsonFile());
}
