/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Connection.h"
#include "circuit/Node.h"
#include "logging/Logger.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Segmentation map.
 */
class SegmentationMap
{
public:
    /** Segmentation map file name. */
    const std::string cSegmentationMapFile{"segmentation_map.json"};

    /**
     * @brief Constructor.
     *
     * @param logger Logger.
     */
    explicit SegmentationMap(const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~SegmentationMap() = default;

    /**
     * @brief Generates the segmentation map, in JSON format.
     *
     * @param components Components.
     * @param connections Connections.
     * @param nodes Nodes.
     *
     * @return True if segmentation map generation occurred successfully, otherwise false.
     */
    virtual bool generateSegmentationMap(const std::vector<circuit::Component>& components,
                                         const std::vector<circuit::Connection>& connections,
                                         const std::vector<circuit::Node>& nodes);

    /**
     * @brief Writes the segmentation map to a JSON file.
     *
     * @note The JSON file is written to the working directory and have the following name: @ref cSegmentationMapFile.
     *
     * @return True if segmentation map was written successfully, otherwise false.
     */
    virtual bool writeSegmentationMapJsonFile();

    /**
     * @brief Gets the segmentation map.
     *
     * @return Segmentation map.
     */
    [[nodiscard]] virtual const nlohmann::ordered_json& getSegmentationMap() const;

private:
    /**
     * @brief Adds the map for components to the segmentation map, in JSON format.
     *
     * @param components Components.
     *
     * @return True if map addition occurred successfully, otherwise false.
     */
    virtual bool addComponentsMap(const std::vector<circuit::Component>& components);

    /**
     * @brief Adds the map for connections to the segmentation map, in JSON format.
     *
     * @param connections Connections.
     *
     * @return True if map addition occurred successfully, otherwise false.
     */
    virtual bool addConnectionsMap(const std::vector<circuit::Connection>& connections);

    /**
     * @brief Adds the map for nodes to the segmentation map, in JSON format.
     *
     * @param nodes Nodes.
     *
     * @return True if map addition occurred successfully, otherwise false.
     */
    virtual bool addNodesMap(const std::vector<circuit::Node>& nodes);

private:
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Segmentation map, in JSON. */
    nlohmann::ordered_json mJsonMap;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
