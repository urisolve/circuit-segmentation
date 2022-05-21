/**
 * @file
 */

#include "application/CommandLineParser.h"
#include <gtest/gtest.h>

/**
 * @brief Test class of CommandLineParser.
 */
class CommandLineParserTest : public ::testing::Test
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

    /** Command line parser. */
    circuitSegmentation::application::CommandLineParser mCommandLineParser{};
};

/**
 * @brief Tests if parser has the help option passed (short option).
 */
TEST_F(CommandLineParserTest, hasHelpShortOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-h"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasHelpOption = mCommandLineParser.hasHelp();

    EXPECT_EQ(true, hasHelpOption);
}

/**
 * @brief Tests if parser has the help option passed (long option).
 */
TEST_F(CommandLineParserTest, hasHelpLongOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--help"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasHelpOption = mCommandLineParser.hasHelp();

    EXPECT_EQ(true, hasHelpOption);
}

/**
 * @brief Tests if parser does not have the help option.
 */
TEST_F(CommandLineParserTest, doesNotHaveHelpOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--h", "-help"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is not present
    const bool hasHelpOption = mCommandLineParser.hasHelp();

    EXPECT_EQ(false, hasHelpOption);
}

/**
 * @brief Tests if parser has the version option passed (short option).
 */
TEST_F(CommandLineParserTest, hasVersionShortOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-v"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasVersionOption = mCommandLineParser.hasVersion();

    EXPECT_EQ(true, hasVersionOption);
}

/**
 * @brief Tests if parser has the version option passed (long option).
 */
TEST_F(CommandLineParserTest, hasVersionLongOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--version"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasVersionOption = mCommandLineParser.hasVersion();

    EXPECT_EQ(true, hasVersionOption);
}

/**
 * @brief Tests if parser does not have the version option.
 */
TEST_F(CommandLineParserTest, doesNotHaveVersionOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--v", "-version"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is not present
    const bool hasVersionOption = mCommandLineParser.hasVersion();

    EXPECT_EQ(false, hasVersionOption);
}

/**
 * @brief Tests if parser has the verbose option passed (short option).
 */
TEST_F(CommandLineParserTest, hasVerboseShortOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-V"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasVerboseOption = mCommandLineParser.hasVerboseLog();

    EXPECT_EQ(true, hasVerboseOption);
}

/**
 * @brief Tests if parser has the verbose option passed (long option).
 */
TEST_F(CommandLineParserTest, hasVerboseLongOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--verbose"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasVerboseOption = mCommandLineParser.hasVerboseLog();

    EXPECT_EQ(true, hasVerboseOption);
}

/**
 * @brief Tests if parser does not have the verbose option.
 */
TEST_F(CommandLineParserTest, doesNotHaveVerboseOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--V", "-verbose"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is not present
    const bool hasVerboseOption = mCommandLineParser.hasVerboseLog();

    EXPECT_EQ(false, hasVerboseOption);
}

/**
 * @brief Tests which value the parser gets for image file path (short option).
 */
TEST_F(CommandLineParserTest, getsImagePathShortOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "-i", "image.png"};

    mCommandLineParser.parse(argc, argv);

    // Verify option value
    const auto value = mCommandLineParser.getImagePath();

    EXPECT_EQ(std::string(argv[2]), value);
}

/**
 * @brief Tests which value the parser gets for image file path (long option).
 */
TEST_F(CommandLineParserTest, getsImagePathLongOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--image", "image.png"};

    mCommandLineParser.parse(argc, argv);

    // Verify option value
    const auto value = mCommandLineParser.getImagePath();

    EXPECT_EQ(std::string(argv[2]), value);
}

/**
 * @brief Tests which value the parser gets for image file path when that option is not passed.
 */
TEST_F(CommandLineParserTest, getsImagePathNoOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--image"};

    mCommandLineParser.parse(argc, argv);

    // Verify option value
    const auto value = mCommandLineParser.getImagePath();

    EXPECT_EQ("", value);
}

/**
 * @brief Tests if parser has the save images option passed (short option).
 */
TEST_F(CommandLineParserTest, hasSaveImagesShortOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "-s"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasSaveImagesOption = mCommandLineParser.hasSaveImages();

    EXPECT_EQ(true, hasSaveImagesOption);
}

/**
 * @brief Tests if parser has the save images option passed (long option).
 */
TEST_F(CommandLineParserTest, hasSaveImagesLongOption)
{
    const int argc = 2;
    const char* argv[] = {"exe", "--save-proc"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is present
    const bool hasSaveImagesOption = mCommandLineParser.hasSaveImages();

    EXPECT_EQ(true, hasSaveImagesOption);
}

/**
 * @brief Tests if parser does not have the save images option.
 */
TEST_F(CommandLineParserTest, doesNotHaveSaveImagesOption)
{
    const int argc = 3;
    const char* argv[] = {"exe", "--s", "-save-proc"};

    mCommandLineParser.parse(argc, argv);

    // Verify if option is not present
    const bool hasSaveImagesOption = mCommandLineParser.hasSaveImages();

    EXPECT_EQ(false, hasSaveImagesOption);
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
