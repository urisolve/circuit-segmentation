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
};

} // namespace circuit
} // namespace circuitSegmentation
