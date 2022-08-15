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
    /**
     * @brief Labels associated to this element.
     *
     * @note As an element can have more than one label, this vector is useful to group all labels associated to this
     * element, to be used on generation of the ROIs of labels.
     */
    std::vector<Label> mLabels{};
};

} // namespace circuit
} // namespace circuitSegmentation
