/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"

namespace circuitSegmentation {
namespace circuit {

/** Alias for wire. */
using Wire = computerVision::Contour;

/**
 * @brief Circuit node.
 */
class Node
{
public:
    /**
     * @brief Constructor.
     */
    Node();

    /**
     * @brief Destructor.
     */
    virtual ~Node() = default;

    /**
     * @brief Sets the wire of the node.
     *
     * @param wire Wire of the node.
     */
    virtual void setWire(const Wire wire);

    /**
     * @brief Gets the wire of the node.
     *
     * @return Wire of the node.
     */
    [[nodiscard]] virtual Wire getWire() const;

private:
    /** Wire of the node. */
    Wire mWire;
};

} // namespace circuit
} // namespace circuitSegmentation
