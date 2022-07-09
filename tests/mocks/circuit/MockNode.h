/**
 * @file
 */

#pragma once

#include "circuit/Node.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Mock of the Node class.
 */
class MockNode : public Node
{
public:
    /** Mocks method setWire. */
    MOCK_METHOD(void, setWire, (const Wire wire), (override));
    /** Mocks method getWire. */
    MOCK_METHOD(Wire, getWire, (), (const, override));
};

} // namespace circuit
} // namespace circuitSegmentation
