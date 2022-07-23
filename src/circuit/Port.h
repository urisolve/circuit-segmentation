/**
 * @file
 */

#pragma once

#include "Id.h"
#include "Position.h"
#include <map>
#include <string>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Connection point of a component.
 */
struct Port
{
    /**
     * @brief Enumeration of the port types allowed.
     */
    enum class PortType : unsigned char {
        /** Hybrid (allow both input and output). */
        HYBRID = 0,
        /** Input. */
        INPUT = 1,
        /** Output. */
        OUTPUT = 2
    };

    /**
     * @brief Constructor.
     */
    Port();

    /**
     * @brief Sets the port type.
     *
     * @param type Type.
     */
    void setType(const PortType& type);

    /** Unique ID. */
    Id mId{};
    /** Owner ID. */
    Id mOwnerId{};
    /** Type. */
    std::string mType{"hybrid"};
    /** Position. */
    RelativePosition mPosition{};
    /** Connection ID. */
    Id mConnectionId{};
};

} // namespace circuit
} // namespace circuitSegmentation
