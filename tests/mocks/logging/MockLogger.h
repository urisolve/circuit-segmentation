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
    /** Mocks method setLogLevel. */
    MOCK_METHOD(void, setLogLevel, (LogLevel level), (override));
    /** Mocks method getLogLevel. */
    MOCK_METHOD(LogLevel, getLogLevel, (), (const, override));
    /** Mocks method logFatal. */
    MOCK_METHOD(void, logFatal, (const std::string& msg), (override));
    /** Mocks method logError. */
    MOCK_METHOD(void, logError, (const std::string& msg), (override));
    /** Mocks method logWarning. */
    MOCK_METHOD(void, logWarning, (const std::string& msg), (override));
    /** Mocks method logInfo. */
    MOCK_METHOD(void, logInfo, (const std::string& msg), (override));
    /** Mocks method logDebug. */
    MOCK_METHOD(void, logDebug, (const std::string& msg), (override));
    /** Mocks method logVerbose. */
    MOCK_METHOD(void, logVerbose, (const std::string& msg), (override));
};

} // namespace logging
} // namespace circuitSegmentation
