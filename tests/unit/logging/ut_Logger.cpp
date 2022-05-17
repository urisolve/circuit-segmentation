/**
 * @file
 */

#include "logging/Logger.h"
#include <chrono>
#include <ctime>
#include <gtest/gtest.h>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

using namespace circuitSegmentation::logging;

/**
 * @brief Test class of Logger.
 */
class LoggerTest : public ::testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mLogger = std::make_shared<Logger>(mStream);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Builds the expected log, without date and time.
     *
     * @param level Log level of the message.
     * @param msg Message to log.
     * @return Expected log message.
     */
    const std::string expectLog(const std::string& level, const std::string& msg)
    {
        std::stringstream ss;
        ss << "[" << level << "] " << msg << std::endl;
        return ss.str();
    }

    /** Logger. */
    std::shared_ptr<Logger> mLogger;
    /** Logger stream */
    std::ostringstream mStream{};

    /** Length of the substring which contains the date and time in the log. */
    static constexpr auto cDateTimeLength{21};
};

/**
 * @brief Tests the log level setted.
 */
TEST_F(LoggerTest, setsLogLevel)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    EXPECT_EQ(Logger::LogLevel::NONE, mLogger->getLogLevel());
}

/**
 * @brief Tests the log for fatal messages.
 */
TEST_F(LoggerTest, logsFatal)
{
    mLogger->setLogLevel(Logger::LogLevel::FATAL);

    const std::string msg{"A fatal message"};
    mLogger->logFatal(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("FATAL", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the fatal level, the message is not logged.
 */
TEST_F(LoggerTest, noFatalLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"A fatal message"};
    mLogger->logFatal(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

/**
 * @brief Tests the log for error messages.
 */
TEST_F(LoggerTest, logsError)
{
    mLogger->setLogLevel(Logger::LogLevel::ERROR);

    const std::string msg{"An error message"};
    mLogger->logError(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("ERROR", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the error level, the message is not logged.
 */
TEST_F(LoggerTest, noErrorLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"An error message"};
    mLogger->logError(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

/**
 * @brief Tests the log for warning messages.
 */
TEST_F(LoggerTest, logsWarning)
{
    mLogger->setLogLevel(Logger::LogLevel::WARNING);

    const std::string msg{"A warning message"};
    mLogger->logWarning(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("WARNING", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the warning level, the message is not logged.
 */
TEST_F(LoggerTest, noWarningLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"A warning message"};
    mLogger->logWarning(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

/**
 * @brief Tests the log for information messages.
 */
TEST_F(LoggerTest, logsInfo)
{
    mLogger->setLogLevel(Logger::LogLevel::INFO);

    const std::string msg{"An information message"};
    mLogger->logInfo(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("INFO", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the information level, the message is not logged.
 */
TEST_F(LoggerTest, noInfoLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"An information message"};
    mLogger->logInfo(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

/**
 * @brief Tests the log for debug messages.
 */
TEST_F(LoggerTest, logsDebug)
{
    mLogger->setLogLevel(Logger::LogLevel::DEBUG);

    const std::string msg{"A debug message"};
    mLogger->logDebug(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("DEBUG", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the debug level, the message is not logged.
 */
TEST_F(LoggerTest, noDebugLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"A debug message"};
    mLogger->logDebug(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

/**
 * @brief Tests the log for verbose messages.
 */
TEST_F(LoggerTest, logsVerbose)
{
    mLogger->setLogLevel(Logger::LogLevel::VERBOSE);

    const std::string msg{"A verbose message"};
    mLogger->logVerbose(msg);

    // Expected message
    const std::string dateTime{mStream.str().substr(0, cDateTimeLength)};
    const std::string expectMsg{dateTime + expectLog("VERBOSE", msg)};

    EXPECT_EQ(expectMsg, mStream.str());
}

/**
 * @brief Tests that if the log level setted is smaller than the verbose level, the message is not logged.
 */
TEST_F(LoggerTest, noVerboseLog)
{
    mLogger->setLogLevel(Logger::LogLevel::NONE);

    const std::string msg{"A verbose message"};
    mLogger->logVerbose(msg);

    EXPECT_EQ(true, mStream.str().empty());
}

// /**
//  * @brief Main function.
//  *
//  * @param argc Number of command line arguments.
//  * @param argv Command line arguments.
//  *
//  * @return 0 on success, 1 on failure.
//  */
// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);

//     return RUN_ALL_TESTS();
// }
