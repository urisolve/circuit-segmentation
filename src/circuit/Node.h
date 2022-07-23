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
};

} // namespace circuit
} // namespace circuitSegmentation
