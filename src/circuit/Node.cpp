/**
 * @file
 */

#include "Node.h"

namespace circuitSegmentation {
namespace circuit {

Node::Node()
    : mWire{}
{
}

void Node::setWire(const Wire wire)
{
    mWire = std::move(wire);
}

Wire Node::getWire() const
{
    return mWire;
}

} // namespace circuit
} // namespace circuitSegmentation
