/**
 * @file
 */

#include "Component.h"

namespace circuitSegmentation {
namespace circuit {

Component::Component()
    : mId{generateId()}
{
    // Set label owner ID
    mLabel.mOwnerId = mId;
}

} // namespace circuit
} // namespace circuitSegmentation
