/**
 * @file
 */

#pragma once

#include <limits>
#include <random>

namespace circuitSegmentation {
namespace common {

/** Alias for engine algorithm. */
using EngineAlgorithm = std::default_random_engine;

/**
 * @brief Simple random number generator.
 *
 * @tparam Int Type of integer.
 */
template<typename Int>
class RandomNumberGen
{
public:
    /**
     * @brief Constructor.
     *
     * @param min Minimum value.
     * @param max Maximum value.
     *
     * @note It is not verified if the minimum and maximum values are correct (e.g. min > max).
     */
    explicit RandomNumberGen(const Int min, const Int max = std::numeric_limits<Int>::max());

    /**
     * @brief Destructor.
     */
    virtual ~RandomNumberGen() = default;

    /**
     * @brief Generate a random integer number.
     *
     * @return Random integer number.
     */
    virtual Int randomNumber();

private:
    /** Non-deterministic random number generator. */
    std::random_device mRd;
    /** Random engine algorithm. */
    EngineAlgorithm mEngine;
    /** Random number distribution. */
    std::uniform_int_distribution<Int> mDist;
};

template<typename Int>
RandomNumberGen<Int>::RandomNumberGen(const Int min, const Int max)
    : mEngine{mRd()}
    , mDist{std::uniform_int_distribution<Int>(std::move(min), std::move(max))}
{
}

template<typename Int>
Int RandomNumberGen<Int>::randomNumber()
{
    // Generate random number
    return mDist(mEngine);
}

} // namespace common
} // namespace circuitSegmentation
