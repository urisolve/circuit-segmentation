/**
 * @file
 */

#include "application/Application.h"

/**
 * @brief Main function.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 *
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char const* argv[])
{
    circuitSegmentation::application::Application app;
    return app.exec(argc, argv);
}
