/**
 * @file
 */

#include "Port.h"

namespace circuitSegmentation {
namespace circuit {

Port::Port()
    : mId{generateId()}
{
}

// LCOV_EXCL_START
// Rationale: Currently this method is not used.
void Port::setType(const Port::PortType& type)
{
    switch (type) {
    case Port::PortType::HYBRID:
        mType = "hybrid";
        break;
    case Port::PortType::INPUT:
        mType = "input";
        break;
    case Port::PortType::OUTPUT:
        mType = "output";
        break;
    default:
        mType = "hybrid";
        break;
    }
}
// LCOV_EXCL_STOP

} // namespace circuit
} // namespace circuitSegmentation
