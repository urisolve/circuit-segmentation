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
     * @brief Constructor.
     *
     * @param ostream Output stream.
     * @param level Log level.
     */
    explicit Logger(std::ostream& ostream, LogLevel level = cLogLevelDefault);

    /**
     * @brief Destructor.
     */
    virtual ~Logger() = default;

    /**
     * @brief Sets the log level.
     *
     * @param level Log level.
     */
    virtual void setLogLevel(LogLevel level);

    /**
     * @brief Gets the log level.
     *
     * @return Log level.
     */
    [[nodiscard]] virtual LogLevel getLogLevel() const;

    /**
     * @brief Logs fatal messages.
     *
     * @param msg Message to log.
     */
    virtual void logFatal(const std::string& msg);

    /**
     * @brief Logs error messages.
     *
     * @param msg Message to log.
     */
    virtual void logError(const std::string& msg);

    /**
     * @brief Logs warning messages.
     *
     * @param msg Message to log.
     */
    virtual void logWarning(const std::string& msg);

    /**
     * @brief Logs information messages.
     *
     * @param msg Message to log.
     */
    virtual void logInfo(const std::string& msg);

    /**
     * @brief Logs debug messages.
     *
     * @param msg Message to log.
     */
    virtual void logDebug(const std::string& msg);

    /**
     * @brief Logs verbose messages.
     *
     * @param msg Message to log.
     */
    virtual void logVerbose(const std::string& msg);

private:
    /**
     * @brief Logs the message.
     *
     * @param level Log level of the message.
     * @param msg Message to log.
     */
    virtual void log(const std::string& level, const std::string& msg);

    /**
     * @brief Gets the current date and time.
     *
     * Format of the output: YYYY-MM-DD HH:MM:SS.
     *
     * @return String with the current date and time.
     */
    virtual const std::string getDateTime() const;

private:
    /** Output stream. */
    std::ostream& mOstream;

    /** Log level. */
    LogLevel mLogLevel{cLogLevelDefault};
};

} // namespace logging
} // namespace circuitSegmentation
