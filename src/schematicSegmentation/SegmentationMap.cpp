/**
 * @file
 */

#include "SegmentationMap.h"
#include "SegmentationUtils.h"
#include <cmath>
#include <fstream>

namespace circuitSegmentation {
namespace schematicSegmentation {

SegmentationMap::SegmentationMap(const std::shared_ptr<logging::Logger>& logger)
    : mLogger{logger}
    , mJsonMap{}
{
}

bool SegmentationMap::generateSegmentationMap(const std::vector<circuit::Component>& components,
                                              const std::vector<circuit::Connection>& connections,
                                              const std::vector<circuit::Node>& nodes)
{
    mLogger->logInfo("Generating segmentation map");

    // Map for elements
    if (!addComponentsMap(components)) {
        return false;
    }
    if (!addNodesMap(nodes)) {
        return false;
    }
    if (!addConnectionsMap(connections)) {
        return false;
    }

    return true;
}

bool SegmentationMap::writeSegmentationMapJsonFile()
{
    mLogger->logInfo("Writing segmentation map JSON file");

    // Create and open file
    std::ofstream file(cSegmentationMapFile, std::ios_base::out);
    if (!file) {
        mLogger->logError("Failed to create and open JSON file");
        return false;
    }

    // Write to file
    file << std::setw(4) << mJsonMap << std::endl;
    if (!file) {
        mLogger->logError("Failed to write JSON file");
        return false;
    }

    return true;
}

const nlohmann::ordered_json& SegmentationMap::getSegmentationMap() const
{
    return mJsonMap;
}

bool SegmentationMap::addComponentsMap(const std::vector<circuit::Component>& components)
{
    mJsonMap["components"] = nlohmann::ordered_json::array();

    auto success{true};

    for (const auto& component : components) {
        try {
            // Component
            nlohmann::ordered_json jsonComponent{};

            // ID
            jsonComponent["id"] = component.mId;
            // Type
            jsonComponent["type"] = component.mType;
            // Full name
            jsonComponent["fullName"] = component.mFullName;
            // Label
            jsonComponent["label"]["id"] = component.mLabel.mId;
            jsonComponent["label"]["owner"] = component.mLabel.mOwnerId;
            jsonComponent["label"]["name"] = component.mLabel.mName;
            jsonComponent["label"]["value"] = component.mLabel.mValue;
            jsonComponent["label"]["unit"] = component.mLabel.mUnit;
            jsonComponent["label"]["position"]["x"] = component.mLabel.mPosition.mX;
            jsonComponent["label"]["position"]["y"] = component.mLabel.mPosition.mY;
            jsonComponent["label"]["position"]["angle"] = component.mLabel.mPosition.mAngle;
            jsonComponent["label"]["isNameHidden"] = component.mLabel.mIsNameHidden;
            jsonComponent["label"]["isValueHidden"] = component.mLabel.mIsValueHidden;

            // Ports
            jsonComponent["ports"] = nlohmann::ordered_json::array();
            for (const auto& port : component.mPorts) {
                // Port
                nlohmann::ordered_json jsonPort{};
                jsonPort["id"] = port.mId;
                jsonPort["owner"] = port.mOwnerId;
                jsonPort["type"] = port.mType;
                jsonPort["position"]["x"] = roundDouble(port.mPosition.mX, 1);
                jsonPort["position"]["y"] = roundDouble(port.mPosition.mY, 1);
                jsonPort["position"]["angle"] = port.mPosition.mAngle;
                jsonPort["connection"] = port.mConnectionId;

                // Add port to ports
                jsonComponent["ports"].push_back(jsonPort);
            }

            // Position
            jsonComponent["position"]["x"] = component.mPosition.mX;
            jsonComponent["position"]["y"] = component.mPosition.mY;
            jsonComponent["position"]["angle"] = component.mPosition.mAngle;

            // Add component to components
            mJsonMap["components"].push_back(jsonComponent);
        }
        catch (const std::exception& ex) {
            mLogger->logError("An exception occurred while generating segmentation map: " + std::string(ex.what()));
            success = false;
            break;
        }
    }

    return success;
}

bool SegmentationMap::addConnectionsMap(const std::vector<circuit::Connection>& connections)
{
    mJsonMap["connections"] = nlohmann::ordered_json::array();

    auto success{true};

    for (const auto& connection : connections) {
        try {
            // Connection
            nlohmann::ordered_json jsonConnection{};

            // ID
            jsonConnection["id"] = connection.mId;
            // Start ID
            jsonConnection["start"] = connection.mStartId;
            // End ID
            jsonConnection["end"] = connection.mEndId;
            // Label
            jsonConnection["label"]["id"] = connection.mLabel.mId;
            jsonConnection["label"]["owner"] = connection.mLabel.mOwnerId;
            jsonConnection["label"]["name"] = connection.mLabel.mName;
            jsonConnection["label"]["value"] = connection.mLabel.mValue;
            jsonConnection["label"]["unit"] = connection.mLabel.mUnit;
            jsonConnection["label"]["position"]["x"] = connection.mLabel.mPosition.mX;
            jsonConnection["label"]["position"]["y"] = connection.mLabel.mPosition.mY;
            jsonConnection["label"]["position"]["angle"] = connection.mLabel.mPosition.mAngle;
            jsonConnection["label"]["isNameHidden"] = connection.mLabel.mIsNameHidden;
            jsonConnection["label"]["isValueHidden"] = connection.mLabel.mIsValueHidden;

            // Add connection to connections
            mJsonMap["connections"].push_back(jsonConnection);
        }
        catch (const std::exception& ex) {
            mLogger->logError("An exception occurred while generating segmentation map: " + std::string(ex.what()));
            success = false;
            break;
        }
    }

    return success;
}

bool SegmentationMap::addNodesMap(const std::vector<circuit::Node>& nodes)
{
    mJsonMap["nodes"] = nlohmann::ordered_json::array();

    auto success{true};

    for (const auto& node : nodes) {
        try {
            // Node
            nlohmann::ordered_json jsonNode{};

            // ID
            jsonNode["id"] = node.mId;
            // Label
            jsonNode["label"]["id"] = node.mLabel.mId;
            jsonNode["label"]["owner"] = node.mLabel.mOwnerId;
            jsonNode["label"]["name"] = node.mLabel.mName;
            jsonNode["label"]["value"] = node.mLabel.mValue;
            jsonNode["label"]["unit"] = node.mLabel.mUnit;
            jsonNode["label"]["position"]["x"] = node.mLabel.mPosition.mX;
            jsonNode["label"]["position"]["y"] = node.mLabel.mPosition.mY;
            jsonNode["label"]["position"]["angle"] = node.mLabel.mPosition.mAngle;
            jsonNode["label"]["isNameHidden"] = node.mLabel.mIsNameHidden;
            jsonNode["label"]["isValueHidden"] = node.mLabel.mIsValueHidden;
            // Position
            jsonNode["position"]["x"] = node.mPosition.mX;
            jsonNode["position"]["y"] = node.mPosition.mY;
            jsonNode["position"]["angle"] = node.mPosition.mAngle;

            // Connections IDs
            jsonNode["connections"] = nlohmann::ordered_json::array();
            for (const auto& connectionId : node.mConnectionIds) {
                // Add connection ID to connections IDs
                jsonNode["connections"].push_back(connectionId);
            }

            // Type
            jsonNode["type"] = node.mType;

            // Add node to nodes
            mJsonMap["nodes"].push_back(jsonNode);
        }
        catch (const std::exception& ex) {
            mLogger->logError("An exception occurred while generating segmentation map: " + std::string(ex.what()));
            success = false;
            break;
        }
    }

    return success;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
