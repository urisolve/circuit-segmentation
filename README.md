# Circuit Segmentation and Modeling

This project aims to interpret and model electrical schematics using computer vision algorithms.

## Table of Contents

- [Circuit Segmentation and Modeling](#circuit-segmentation-and-modeling)
    - [Project structure](#project-structure)
    - [Compilation](#compilation)
    - [Tests](#tests)
    - [Code coverage](#code-coverage)
    - [Documentation](#documentation)
    - [Code formatting and analysis](#code-formatting-and-analysis)

## Project structure

This project has the following structure:

- circuit-examples/: contains some circuit examples to be used by the application
- cmake/: contains useful CMake files
- docs/: contains documentation related with the software architecture, and some circuit examples
- doxygen/: contains the doxyfile used to build documentation
- scripts/: contains useful scripts
- src/: contains the source code of the project
- tests/: contains files related with tests
- .clang-format: file with the code format rules
- .clang-tidy: file with the code analysis checks
- .gitignore: file that specifies intentionally untracked files that Git should ignore
- CMakeLists.txt: root CMake file
- LICENSE: license file
- README.md: README file of this project

## Dependencies

The following libraries are used in this project:

- **OpenCV 4.5.5**: needs to be installed on the system
- **GoogleTest**: fetched with CMake (if BUILD_TESTS is ON)

## Compilation

This project uses the CMake tool for build configuration. The CMake options for this project are:

| CMake Option | Description | Default value |
| --- | --- | --- |
| CMAKE_BUILD_TYPE | Build type on single-configuration generators. <br /> Typical values include Debug, Release, RelWithDebInfo and MinSizeRel. | Debug |
| BUILD_TESTS | Build unit tests | ON |
| BUILD_COVERAGE | Build with code coverage | OFF |

The following commands can be used to compile this project (using generator for MinGW makefiles):
```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
$ cmake --build .
```

## Tests

To run the unit tests, use the commands below (using generator for MinGW makefiles):
```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -G "MinGW Makefiles"
$ cmake --build .
$ ctest
```

## Code coverage

The project can be compiled for code coverage analysis, using the following commands (using generator for MinGW makefiles):
```sh
$ cd <project-directory>
$ mkdir build-coverage
$ cd build-coverage
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_COVERAGE=ON -G "MinGW Makefiles"
$ cmake --build .
```

For generation of the report, use the correspondent script as follows:
```sh
$ cd <project-directory>
$ ./scripts/coverage-gcovr-gen.sh build-coverage
```

The report file named `coverage.html` is created in `build-coverage/coverage` directory.

## Documentation

This project uses the Doxygen tool for documentation generation. The correspondent script can be used to generate the documentation:
```sh
$ cd <project-directory>
$ ./scripts/doxygen-doc-gen.sh ./doxygen/Doxyfile
```

The documentation is created and can be accessed in `build-doxygen/html/index.html`.

## Code formatting and analysis

This projects uses clang-format tool for code formatting. The format of the code can be checked using the correspondent script:
```sh
$ cd <project-directory>
$ # To check the src directory
$ ./scripts/clang-format-check.sh ./src
```

For code static analysis, this project uses clang-tidy tool. The correspondent script can be used to analyze the code:
```sh
$ cd <project-directory>
$ ./scripts/clang-tidy-check.sh
```

A report file is created in `build-clang-tidy` directory.
