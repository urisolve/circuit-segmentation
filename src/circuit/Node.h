/**
 * @file
 */

#pragma once

#include "Id.h"
#include "Label.h"
#include "Position.h"
#include <map>
#include <string>
#include <vector>

namespace circuitSegmentation {
namespace circuit {

/**
 * @brief Circuit node.
 *
 * It represents connection points.
 */
struct Node
{
    /**
     * @brief Enumeration of the node types allowed.
     */
    enum class NodeType : unsigned char {
        /** Real (node holds more than 2 connections). */
        REAL = 0,
        /** Virtual. */
        VIRTUAL = 1
    };

    /**
     * @brief Constructor.
     */
    Node();

    /**
     * @brief Sets the node type.
     *
     * @param type Type.
     */
    void setType(const NodeType& type);

    /** Unique ID. */
    Id mId{};
    /** Type. */
    std::string mType{"real"};
    /** Position. */
    GlobalPosition mPosition{};
    /** Connection IDs. */
    std::vector<Id> mConnectionIds{};
    /** Label. */
    Label mLabel{};

    /**
     * @brief Labels associated to this element.
     *
     * @note As an element can have more than one label, this vector is useful to group all labels associated to this
     * element, to be used on generation of the ROIs of labels.
     */
    std::vector<Label> mLabels{};
};

} // namespace circuit
} // namespace circuitSegmentation
