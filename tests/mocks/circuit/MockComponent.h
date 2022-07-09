/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Mock of the Component class.
 */
class MockComponent : public Component
{
public:
    /** Mocks method setBoundingBox. */
    MOCK_METHOD(void, setBoundingBox, (const computerVision::Rectangle box), (override));
    /** Mocks method getBoundingBox. */
    MOCK_METHOD(computerVision::Rectangle, getBoundingBox, (), (const, override));
    /** Mocks method setNodes. */
    MOCK_METHOD(void, setNodes, (const std::vector<Node> nodes), (override));
    /** Mocks method getNodes. */
    MOCK_METHOD(std::vector<Node>, getNodes, (), (const, override));
};

} // namespace circuit
} // namespace circuitSegmentation
