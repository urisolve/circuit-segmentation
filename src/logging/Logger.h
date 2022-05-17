/**
 * @file
 */

#pragma once

#include <ostream>

namespace circuitSegmentation {
namespace logging {

/**
 * @brief Simple logger.
 */
class Logger
{
public:
    /**
     * @brief Enumeration of the log levels.
     *
     * The log level defines the level for the logger.
     * For example, if the log level is setted to INFO, the levels above (DEBUG and VERBOSE) will not be logged.
     */
    enum class LogLevel : unsigned char {
        /** No messages. */
        NONE = 0,
        /** Fatal message. */
        FATAL = 1,
        /** Error message. */
        ERROR = 2,
        /** Warning message. */
        WARNING = 3,
        /** Information message. */
        INFO = 4,
        /** Debug message. */
        DEBUG = 5,
        /** Verbose message. */
        VERBOSE = 6
    };

    /** Default log level. */
    static constexpr auto cLogLevelDefault{LogLevel::VERBOSE};

    /**
     * @brief Constructor of a new object.
     *
     * @param ostream Output stream.
     * @param level Log level.
     */
    Logger(std::ostream& ostream, LogLevel level = cLogLevelDefault);

    /**
     * @brief Sets the log level.
     *
     * @param level Log level.
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Gets the log level.
     *
     * @return Log level.
     */
    [[nodiscard]] LogLevel getLogLevel() const;

    /**
     * @brief Logs fatal messages.
     *
     * @param msg Message to log.
     */
    void logFatal(const std::string& msg);

    /**
     * @brief Logs error messages.
     *
     * @param msg Message to log.
     */
    void logError(const std::string& msg);

    /**
     * @brief Logs warning messages.
     *
     * @param msg Message to log.
     */
    void logWarning(const std::string& msg);

    /**
     * @brief Logs information messages.
     *
     * @param msg Message to log.
     */
    void logInfo(const std::string& msg);

    /**
     * @brief Logs debug messages.
     *
     * @param msg Message to log.
     */
    void logDebug(const std::string& msg);

    /**
     * @brief Logs verbose messages.
     *
     * @param msg Message to log.
     */
    void logVerbose(const std::string& msg);

private:
    /** Output stream. */
    std::ostream& mOstream;

    /** Log level. */
    LogLevel mLogLevel{cLogLevelDefault};

    /**
     * @brief Logs the message.
     *
     * @param level Log level of the message.
     * @param msg Message to log.
     */
    void log(const std::string& level, const std::string& msg);

    /**
     * @brief Gets the current date and time.
     *
     * Format of the output: YYYY-MM-DD HH:MM:SS.
     *
     * @return String with the current date and time.
     */
    const std::string getDateTime() const;
};

} // namespace logging
} // namespace circuitSegmentation
