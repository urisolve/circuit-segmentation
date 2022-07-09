/**
 * @file
 */

#include "Component.h"

namespace circuitSegmentation {
namespace circuit {

Component::Component()
    : mBoundingBox{}
    , mNodes{}
{
}

void Component::setBoundingBox(const computerVision::Rectangle box)
{
    mBoundingBox = std::move(box);
}

computerVision::Rectangle Component::getBoundingBox() const
{
    return mBoundingBox;
}

void Component::setNodes(const std::vector<Node> nodes)
{
    mNodes = std::move(nodes);
}

std::vector<Node> Component::getNodes() const
{
    return mNodes;
}

} // namespace circuit
} // namespace circuitSegmentation
