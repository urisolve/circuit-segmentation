/**
 * @file
 */

#pragma once

#include "logging/Logger.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace logging {

/**
 * @brief Mock of the Logger class.
 */
class MockLogger : public Logger
{
public:
    /**
     * @brief Constructor of a new object.
     *
     * @param ostream Output stream.
     * @param level Log level.
     */
    MockLogger(std::ostream& ostream, LogLevel level = cLogLevelDefault)
        : Logger(ostream, level)
    {
    }

    /** Mocks method "void setLogLevel(LogLevel level)". */
    MOCK_METHOD(void, setLogLevel, (LogLevel level), ());
    /** Mocks method "LogLevel getLogLevel() const". */
    MOCK_METHOD(LogLevel, getLogLevel, (), (const));
    /** Mocks method "void logFatal(const std::string& msg)". */
    MOCK_METHOD(void, logFatal, (const std::string& msg), ());
    /** Mocks method "void logError(const std::string& msg)". */
    MOCK_METHOD(void, logError, (const std::string& msg), ());
    /** Mocks method "void logWarning(const std::string& msg)". */
    MOCK_METHOD(void, logWarning, (const std::string& msg), ());
    /** Mocks method "void logInfo(const std::string& msg)". */
    MOCK_METHOD(void, logInfo, (const std::string& msg), ());
    /** Mocks method "void logDebug(const std::string& msg)". */
    MOCK_METHOD(void, logDebug, (const std::string& msg), ());
    /** Mocks method "void logVerbose(const std::string& msg)". */
    MOCK_METHOD(void, logVerbose, (const std::string& msg), ());
};

} // namespace logging
} // namespace circuitSegmentation
