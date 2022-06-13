/**
 * @file
 */

#pragma once

#include "common/RandomNumberGen.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace common {

/**
 * @brief Mock of the RandomNumberGen class.
 *
 * @tparam Int Type of integer.
 */
template<typename Int>
class MockRandomNumberGen : public RandomNumberGen<Int>
{
public:
    /**
     * @brief Constructor.
     *
     * @param min Minimum value.
     * @param max Maximum value
     */
    explicit MockRandomNumberGen(const Int min, const Int max = std::numeric_limits<Int>::max())
        : RandomNumberGen(min, max)
    {
    }

    /** Mocks method randomNumber. */
    MOCK_METHOD(Int, randomNumber, (), (override));
};

} // namespace common
} // namespace circuitSegmentation
