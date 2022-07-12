/**
 * @file
 */

#pragma once

#include "uuid.h"
#include <string>

namespace circuitSegmentation {
namespace common {

/**
 * @brief Universally Unique Identifier (UUID) generator.
 */
class UuidGen
{
public:
    /**
     * @brief Constructor.
     */
    UuidGen() = default;

    /**
     * @brief Destructor.
     */
    virtual ~UuidGen() = default;

    /**
     * @brief Generate a new UUID.
     *
     * @return New UUID.
     */
    virtual uuids::uuid generateUuid() const;

    /**
     * @brief Generate a new UUID as string.
     *
     * @return New UUID.
     */
    virtual std::string generateStringUuid() const;
};

} // namespace common
} // namespace circuitSegmentation
