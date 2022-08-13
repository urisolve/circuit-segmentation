/**
 * @file
 */

#include "Connection.h"

namespace circuitSegmentation {
namespace circuit {

Connection::Connection()
    : mId{generateId()}
{
    // Set label owner ID
    mLabel.mOwnerId = mId;
}

} // namespace circuit
} // namespace circuitSegmentation
