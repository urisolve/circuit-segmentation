/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "Connection.h"
#include "Id.h"
#include "Label.h"
#include "Port.h"
#include "Position.h"
#include <string>
#include <vector>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Circuit component.
 *
 * It represents any kind of electrical component, such as resistor, capacitor, inductor and voltage source.
 */
struct Component
{
    /**
     * @brief Constructor.
     */
    Component();

    /** Unique ID. */
    Id mId{};
    /** Type of component (example: "R" for resistor). */
    std::string mType{""};
    /** Full name of component type (example: "Resistor" for resistor). */
    std::string mFullName{""};
    /** Position. */
    GlobalPosition mPosition{};
    /** Label. */
    Label mLabel{};
    /** Ports of component. */
    std::vector<Port> mPorts{};

    /** Bounding box of the component. */
    computerVision::Rectangle mBoundingBox{};
    /** Connections to the component. */
    std::vector<Connection> mConnections{};
};

} // namespace circuit
} // namespace circuitSegmentation
