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
    virtual ~Position() = default;

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
    virtual ~GlobalPosition() = default;
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
    virtual ~RelativePosition() = default;
};

} // namespace circuit
} // namespace circuitSegmentation
