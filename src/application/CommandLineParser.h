/**
 * @file
 */

#pragma once

#include "utils/CmdLineParser.h"
#include <string>

namespace circuitSegmentation {
namespace application {

/**
 * @brief Parser of command line arguments.
 *
 * The application runs with the following command line arguments:
 * - -h, --help: show help message
 * - -v, --version: show application version
 * - -V, --verbose: enable verbose logs
 * - -i, --image: image file path with the circuit
 * - -s, --save-proc: save images obtained during the processing in the working directory
 */
class CommandLineParser
{
public:
    /**
     * @brief Parses the command line arguments.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    void parse(const int argc, char const* argv[]);

    /**
     * @brief Checks if help option was passed.
     *
     * @return Returns true if the option was passed, otherwise false.
     */
    [[nodiscard]] bool hasHelp() const;

    /**
     * @brief Checks if version option was passed.
     *
     * @return Returns true if the option was passed, otherwise false.
     */
    [[nodiscard]] bool hasVersion() const;

    /**
     * @brief Checks if verbose option was passed.
     *
     * @return Returns true if the option was passed, otherwise false.
     */
    [[nodiscard]] bool hasVerboseLog() const;

    /**
     * @brief Gets image file path option passed.
     *
     * @return Image file path passed, or an empty string if option was not passed.
     */
    [[nodiscard]] std::string getImagePath() const;

    /**
     * @brief Checks if save images option was passed.
     *
     * @return Returns true if the option was passed, otherwise false.
     */
    [[nodiscard]] bool hasSaveImages() const;

private:
    /** Parser of the command line arguments. */
    utils::CmdLineParser mParser{};
};

} // namespace application
} // namespace circuitSegmentation
