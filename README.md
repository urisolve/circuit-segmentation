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

- cmake/: contains useful CMake files
- diagrams/: contains diagrams related with the software architecture
- doxygen/: contains the doxyfile used to build documentation
- scripts/: contains useful scripts
- src/: contains the source code of the project
- tests/: contains files related with tests
- .clang-format: file with the code format rules
- .clang-tidy: file with the code analysis checks
- .gitignore: file that specifies intentionally untracked files that Git should ignore
- CMakeLists.txt: root CMake file
- README.md: README file of this project

## Dependencies

The following libraries are used in this project:

- OpenCV 4.5.5

## Compilation

This project uses the CMake tool for build configuration. The CMake options for this project are:

| CMake Option | Description | Default value |
| --- | --- | --- |
| CMAKE_BUILD_TYPE | Build type on single-configuration generators. <br /> Typical values include Debug, Release, RelWithDebInfo and MinSizeRel. | Debug |
| BUILD_TESTS | Build unit tests | ON |
| BUILD_COVERAGE | Build with code coverage | OFF |

The following commands can be used to compile this project (using MinGW-W64):
```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
$ mingw32-make
```

## Tests

To run the unit tests, use the commands below (using MinGW-W64):
```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -G "MinGW Makefiles"
$ mingw32-make
$ mingw32-make test
```

It can also be used CMake to run the unit tests (use `--verbose` to have more output information):
```sh
$ ctest
```

## Code coverage

The project can be compiled for code coverage analysis, using the following commands (using MinGW-W64):
```sh
$ cd <project-directory>
$ mkdir build-coverage
$ cd build-coverage
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_COVERAGE=ON -G "MinGW Makefiles"
$ mingw32-make
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

The documentation is created in `doc` directory in the root of the project, and the index can be accessed in `doc/html/index.html`.

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
$ # To analyze the src directory
$ ./scripts/clang-tidy-check.sh ./src
```
