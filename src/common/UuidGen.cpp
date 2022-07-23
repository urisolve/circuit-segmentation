/**
 * @file
 */

#define UUID_SYSTEM_GENERATOR

#include "UuidGen.h"

namespace circuitSegmentation {
namespace common {

uuids::uuid UuidGen::generateUuid() const
{
    const uuids::uuid id = uuids::uuid_system_generator{}();
    return id;
}

std::string UuidGen::generateStringUuid() const
{
    const auto id = generateUuid();

    return uuids::to_string(id);
}

} // namespace common
} // namespace circuitSegmentation
