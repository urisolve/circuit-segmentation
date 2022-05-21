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
    /** Mocks method "virtual void setLogLevel(LogLevel level)". */
    MOCK_METHOD(void, setLogLevel, (LogLevel level), (override));
    /** Mocks method "virtual LogLevel getLogLevel() const". */
    MOCK_METHOD(LogLevel, getLogLevel, (), (const, override));
    /** Mocks method "virtual void logFatal(const std::string& msg)". */
    MOCK_METHOD(void, logFatal, (const std::string& msg), (override));
    /** Mocks method "virtual void logError(const std::string& msg)". */
    MOCK_METHOD(void, logError, (const std::string& msg), (override));
    /** Mocks method "virtual void logWarning(const std::string& msg)". */
    MOCK_METHOD(void, logWarning, (const std::string& msg), (override));
    /** Mocks method "virtual void logInfo(const std::string& msg)". */
    MOCK_METHOD(void, logInfo, (const std::string& msg), (override));
    /** Mocks method "virtual void logDebug(const std::string& msg)". */
    MOCK_METHOD(void, logDebug, (const std::string& msg), (override));
    /** Mocks method "virtual void logVerbose(const std::string& msg)". */
    MOCK_METHOD(void, logVerbose, (const std::string& msg), (override));
};

} // namespace logging
} // namespace circuitSegmentation
