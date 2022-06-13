/**
 * @file
 */

#include "cmdLineParser/CmdLineParser.h"
#include <gtest/gtest.h>

/**
 * @brief Test class of CmdLineParser.
 */
class CmdLineParserTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override {}

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Command line parser. */
    circuitSegmentation::cmdLineParser::CmdLineParser mCmdLineParser{};
};

/**
 * @brief Tests if the command line arguments are parsed correctly.
 */
TEST_F(CmdLineParserTest, parsesCommandLineArgs)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--help"};

    mCmdLineParser.parse(argc, argv);

    // Verify command line args
    const auto cmdLineArgs = mCmdLineParser.getArgs();

    EXPECT_EQ(argc, cmdLineArgs.size());
    EXPECT_EQ(std::string(argv[0]), cmdLineArgs.at(0));
    EXPECT_EQ(std::string(argv[1]), cmdLineArgs.at(1));
}

/**
 * @brief Tests if parser has the command line option passed.
 */
TEST_F(CmdLineParserTest, hasCommandOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-h"};

    mCmdLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasOptionArgs = mCmdLineParser.hasOption(std::string(argv[1]));

    EXPECT_TRUE(hasOptionArgs);
}

/**
 * @brief Tests if parser does not have a command line option not passed.
 */
TEST_F(CmdLineParserTest, doesNotHaveCommandOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-h"};

    mCmdLineParser.parse(argc, argv);

    // Verify if option is not present
    const bool hasOptionArgs = mCmdLineParser.hasOption("-v");

    EXPECT_FALSE(hasOptionArgs);
}

/**
 * @brief Tests if parser gets the command line option value passed.
 */
TEST_F(CmdLineParserTest, getsCommandOptionValue)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--param", "1"};

    mCmdLineParser.parse(argc, argv);

    // Verify option value
    const std::string value = mCmdLineParser.getOption(std::string(argv[1]));

    EXPECT_EQ(std::string(argv[2]), value);
}

/**
 * @brief Tests if parser returns an empty string when the command line option value is not passed.
 */
TEST_F(CmdLineParserTest, getsCommandOptionEmpty)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--param"};

    mCmdLineParser.parse(argc, argv);

    // Verify option value
    const std::string value = mCmdLineParser.getOption(std::string(argv[1]));

    EXPECT_EQ("", value);
}

/**
 * @brief Tests the application name setted.
 */
TEST_F(CmdLineParserTest, setsAppName)
{
    const std::string appName = "Application Name";

    mCmdLineParser.setAppName(appName);

    EXPECT_EQ(appName, mCmdLineParser.getAppName());
}

/**
 * @brief Tests the application version setted.
 */
TEST_F(CmdLineParserTest, setsAppVersion)
{
    const std::string appVersion = "1.0.0";

    mCmdLineParser.setAppVersion(appVersion);

    EXPECT_EQ(appVersion, mCmdLineParser.getAppVersion());
}

/**
 * @brief Tests the application description setted.
 */
TEST_F(CmdLineParserTest, setsAppDescription)
{
    const std::string appDescription = "Some description for this application";

    mCmdLineParser.setAppDescription(appDescription);

    EXPECT_EQ(appDescription, mCmdLineParser.getAppDescription());
}

/**
 * @brief Tests the application usage information setted.
 */
TEST_F(CmdLineParserTest, setsAppUsageInfo)
{
    const std::string exeName = "exe";
    const std::string optionsUsageInfo = "[OPTION] -f file";
    const std::map<std::string, std::string> options = {
        {"-h, --help", "show help message"},
        {"-v, --version", "show version"},
        {"-f, --file", "output file"},
    };

    mCmdLineParser.setAppUsageInfo(exeName, optionsUsageInfo, options);

    EXPECT_EQ(exeName, mCmdLineParser.getExeAppName());
    EXPECT_EQ(optionsUsageInfo, mCmdLineParser.getOptionsUsageInfo());
    EXPECT_EQ(options, mCmdLineParser.getOptions());
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
