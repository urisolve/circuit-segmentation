/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "Id.h"
#include "Position.h"

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Label for each circuit element.
 *
 * It holds the text that visually differentiates each element.
 */
struct Label
{
    /**
     * @brief Constructor.
     */
    Label();

    /** Unique ID. */
    Id mId{};
    /** Owner ID. */
    Id mOwnerId{};
    /** Name. */
    std::string mName{""};
    /** Value. */
    std::string mValue{""};
    /** Unit. */
    std::string mUnit{""};
    /** Position. */
    GlobalPosition mPosition{};
    /** Flag that controls if name is hidden. */
    bool mIsNameHidden{true};
    /** Flag that controls if value is hidden. */
    bool mIsValueHidden{true};

    /** Bounding box of the label. */
    computerVision::Rectangle mBoundingBox{};
};

} // namespace circuit
} // namespace circuitSegmentation
