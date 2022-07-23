/**
 * @file
 */

#include "Node.h"

namespace circuitSegmentation {
namespace circuit {

Node::Node()
    : mId{generateId()}
{
}

void Node::setType(const Node::NodeType& type)
{
    switch (type) {
    case Node::NodeType::REAL:
        mType = "real";
        break;
    case Node::NodeType::VIRTUAL:
        mType = "virtual";
        break;
    default:
        mType = "virtual";
        break;
    }
}

} // namespace circuit
} // namespace circuitSegmentation
