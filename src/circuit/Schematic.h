/**
 * @file
 */

#pragma once

#include "Component.h"
#include "Connection.h"
#include "Node.h"
#include <vector>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Circuit schematic.
 */
struct Schematic
{
    /**
     * @brief Constructor.
     */
    Schematic();

    /** Components. */
    std::vector<Component> mComponents{};
    /** Connections. */
    std::vector<Connection> mConnections{};
    /** Nodes. */
    std::vector<Node> mNodes{};
};

} // namespace circuit
} // namespace circuitSegmentation
