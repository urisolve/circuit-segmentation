/**
 * @file
 */

#pragma once

#include <map>
#include <string>
#include <vector>

namespace circuitSegmentation {
namespace cmdLineParser {

/**
 * @brief Parser of command line arguments.
 */
class CmdLineParser
{
public:
    /**
     * @brief Constructor.
     */
    CmdLineParser();

    /**
     * @brief Parses the command line arguments.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    void parse(const int argc, char const* argv[]);

    /**
     * @brief Gets the command line arguments.
     *
     * @return Vector with the command line arguments.
     */
    [[nodiscard]] std::vector<std::string> getArgs() const;

    /**
     * @brief Checks if the command line arguments has the option specified.
     *
     * This method is useful to check if, for example, a `<option>` is passed to the command line arguments.
     *
     * @param option Command line option to check if it was passed.
     *
     * @return True if the option was passed, otherwise false.
     */
    bool hasOption(const std::string& option) const;

    /**
     * @brief Gets the value for the option specified.
     *
     * This method is useful to check if, for example, a `<option> <option_value>` is passed to the command line
     * arguments, and gets its value.
     *
     * @param option Command line option to check if it was passed.
     *
     * @return Command line option value if the option was passed, otherwise an empty string.
     */
    std::string getOption(const std::string& option) const;

    /**
     * @brief Sets the application name.
     *
     * The application name is used when the version option is requested.
     *
     * @param name Application name.
     */
    void setAppName(const std::string& name);

    /**
     * @brief Gets the application name.
     *
     * @return Application name.
     */
    [[nodiscard]] std::string getAppName() const;

    /**
     * @brief Sets the application version.
     *
     * @param version Application version.
     */
    void setAppVersion(const std::string& version);

    /**
     * @brief Gets the application version.
     *
     * @return Application version.
     */
    [[nodiscard]] std::string getAppVersion() const;

    /**
     * @brief Sets the application description.
     *
     * The application description is used when the version option is requested.
     *
     * @param description Application description.
     */
    void setAppDescription(const std::string& description);

    /**
     * @brief Gets the application description.
     *
     * @return Application description.
     */
    [[nodiscard]] std::string getAppDescription() const;

    /**
     * @brief Shows the version information.
     */
    void showVersion() const;

    /**
     * @brief Sets the application usage information.
     *
     * Example:
     * - exeName: app
     * - optionsUsageInfo: [OPTION]
     * - options: "-h, --help", "show help message"
     * - Help message:
     * @code
     * Usage: app [OPTION]
     *
     *     -h, --help        show help message
     * @endcode

     * @param exeName Executable name of the application.
     * @param optionsUsageInfo Usage information of the options.
     * @param options Map with the options.
     */
    void setAppUsageInfo(const std::string& exeName,
                         const std::string& optionsUsageInfo,
                         const std::map<std::string, std::string>& options);

    /**
     * @brief Gets the executable name of the application.
     *
     * @return Executable name of the application.
     */
    [[nodiscard]] std::string getExeAppName() const;

    /**
     * @brief Gets the usage information of the options.
     *
     * @return Usage information of the options.
     */
    [[nodiscard]] std::string getOptionsUsageInfo() const;

    /**
     * @brief Gets the command line options defined to be used in the help message.
     *
     * @return Map with the options.
     */
    [[nodiscard]] std::map<std::string, std::string> getOptions() const;

    /**
     * @brief Shows the help information.
     */
    void showHelp() const;

private:
    /** Command line arguments. */
    std::vector<std::string> mCmdLineArgs{};

    /** Application name. */
    std::string mAppName{""};
    /** Application version. */
    std::string mAppVersion{""};
    /** Application description. */
    std::string mAppDescription{""};

    /** Executable name of the application. */
    std::string mExeAppName{""};
    /** Usage information of the options. */
    std::string mOptionsUsageInfo{""};
    /** Usage information of the options. */
    std::map<std::string, std::string> mOptions{};
};

} // namespace cmdLineParser
} // namespace circuitSegmentation
