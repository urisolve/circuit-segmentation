/**
 * @file
 */

#pragma once

#include "common/UuidGen.h"
#include <string>

namespace circuitSegmentation {
namespace circuit {

using Id = std::string;

/**
 * @brief Generate unique ID.
 *
 * @return Unique ID.
 */
inline const Id generateId()
{
    const common::UuidGen uuidGen{};
    return uuidGen.generateStringUuid();
}

} // namespace circuit
} // namespace circuitSegmentation
