/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "Id.h"
#include "Label.h"

namespace circuitSegmentation {
namespace circuit {

/** Alias for wire. */
using Wire = computerVision::Contour;

/**
 * @brief Circuit connection.
 *
 * It represents the physical wire connection between components' ports and nodes.
 */
struct Connection
{
    /**
     * @brief Constructor.
     */
    Connection();

    /** Unique ID. */
    Id mId{};
    /** Start ID (port or node ID). */
    Id mStartId{};
    /** End ID (port or node ID). */
    Id mEndId{};
    /** Label. */
    Label mLabel{};

    /** Connection wire. */
    Wire mWire{};
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
