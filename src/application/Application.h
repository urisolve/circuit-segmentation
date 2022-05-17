/**
 * @file
 */

#pragma once

namespace circuitSegmentation {
namespace application {

/**
 * @brief Application class.
 */
class Application
{
public:
    /** Application name. */
    static constexpr auto cAppName{"Circuit Segmentation and Modeling"};
    /** Application version, with format major.minor.patch. */
    static constexpr auto cAppVersion{"1.0.0"};
    /** Application date. */
    static constexpr auto cAppDate{"2022"};
    /** Application description. */
    static constexpr auto cAppDescription{
        "Interpretation and modeling of electrical schematics using computer vision algorithms"};
    /** Application executable name. */
    static constexpr auto cAppExeName{"CircuitSegmentation"};

    /**
     * @brief Executes the application.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     *
     * @return Process error: 0 on success, 1 on failure.
     */
    int exec(int& argc, char const* argv[]);
};

} // namespace application
} // namespace circuitSegmentation
