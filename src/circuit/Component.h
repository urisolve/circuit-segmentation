/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "Node.h"

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Circuit component.
 */
class Component
{
public:
    /**
     * @brief Constructor.
     */
    Component();

    /**
     * @brief Destructor.
     */
    virtual ~Component() = default;

    /**
     * @brief Sets the component bounding box.
     *
     * @param box Component bounding box.
     */
    virtual void setBoundingBox(const computerVision::Rectangle box);

    /**
     * @brief Gets the component bounding box.
     *
     * @return Component bounding box.
     */
    [[nodiscard]] virtual computerVision::Rectangle getBoundingBox() const;

    /**
     * @brief Sets the nodes to which component is connected.
     *
     * @param nodes Nodes to which component is connected.
     */
    virtual void setNodes(const std::vector<Node> nodes);

    /**
     * @brief Gets the nodes to which component is connected.
     *
     * @return Nodes to which component is connected.
     */
    [[nodiscard]] virtual std::vector<Node> getNodes() const;

private:
    /** Bounding box of the component. */
    computerVision::Rectangle mBoundingBox;

    /** Nodes to which the component is connected. */
    std::vector<Node> mNodes;
};

} // namespace circuit
} // namespace circuitSegmentation
