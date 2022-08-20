/**
 * @file
 */

#pragma once

#include <optional>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Position for each circuit element.
 *
 * @tparam T Type of coordinates.
 */
template<typename T>
struct Position
{
    /**
     * @brief Constructor.
     */
    Position() = default;

    /**
     * @brief Destructor.
     */
    virtual ~Position() = default; // LCOV_EXCL_LINE

    /** X coordinate. */
    T mX{};
    /** Y coordinate. */
    T mY{};
    /** Angle. */
    int mAngle{};
};

/**
 * @brief Global position for each circuit element.
 */
struct GlobalPosition : public Position<int>
{
    /**
     * @brief Constructor.
     */
    GlobalPosition() = default;

    /**
     * @brief Destructor.
     */
    virtual ~GlobalPosition() = default; // LCOV_EXCL_LINE
};

/**
 * @brief Relative position for each circuit element.
 */
struct RelativePosition : public Position<double>
{
    /**
     * @brief Constructor.
     */
    RelativePosition() = default;

    /**
     * @brief Destructor.
     */
    virtual ~RelativePosition() = default; // LCOV_EXCL_LINE
};

} // namespace circuit
} // namespace circuitSegmentation
