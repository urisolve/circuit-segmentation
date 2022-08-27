# Circuit Segmentation and Modeling

This project consists of a software module capable of interpreting and modeling electrical schematics from an image (drawn by hand or by computer), using computer vision algorithms.

The elements that this software is able to segment are:

- Components
- Nodes
- Connections between the circuit elements
- Labels (like names and values) associated to the circuit elements

This software module generates:

- A JSON file with a map resulting from the segmentation of the electrical schematic (structure respects the [model](./docs/segmentation-map/segmentation-map.md) used in the Circuit Editor)
- Image files with the segmented components and labels (to be used by another software module to classify the components)

## Table of contents

- [Pre-requisites](#pre-requisites)
    - [Using Docker](#using-docker)
    - [Installing locally](#installing-locally)
- [Compilation](#compilation)
- [Running](#running)
- [Tests](#tests)
- [Documentation](#documentation)
- [Supported compilers](#supported-compilers)
- [Used third-party tools](#used-third-party-tools)
    - [Build and test automation](#build-and-test-automation)
    - [Code coverage](#code-coverage)
    - [Doxygen documentation](#doxygen-documentation)
    - [Code formatting and analysis](#code-formatting-and-analysis)
- [License](#license)

## Pre-requisites

Download the software in form of an archive or clone the repository, using the following command:

```sh
$ git clone https://github.com/urisolve/circuit-segmentation.git
```

### Using Docker

There is a docker image available for this project that contains all the dependencies to compile and run this software module. The instructions to use it can be found [here](./docker/README.md). For this approach, the tool that needs to be installed on the system is:

- [Docker](https://docs.docker.com/get-docker/): platform for developing, shipping, and running applications

### Installing locally

These tools need to be installed on the system:

- [CMake](https://cmake.org/): system to manage the build process
- C++ compiler: for software compilation (examples: Microsoft Visual C++, GCC and Clang)

The following library must also be installed:
- [**OpenCV**](https://github.com/opencv/opencv): Open Source Computer Vision Library
    - Version used during the development of this project: 4.5.5

The next libraries are used by the project but are automatically fetched during CMake configuration, so there is no need to install them manually:
- [**GoogleTest**](https://github.com/google/googletest): Google Testing and Mocking Framework
    - Automatically fetched if `BUILD_TESTS` option is ON
    - Version used during the development of this project: 1.11.0
- [**stduuid**](https://github.com/mariusbancila/stduuid): a C++17 cross-platform implementation for universally unique identifiers (UUIDs)
    - Version used during the development of this project: commit hash `3afe7193facd5d674de709fccc44d5055e144d7a`
- [**json**](https://github.com/nlohmann/json): JSON for Modern C++
    - Version used during the development of this project: 3.11.2

## Compilation

The CMake options for configuration of this project are:

| CMake Option | Description | Default value |
| --- | --- | --- |
| CMAKE_BUILD_TYPE | Build type on single-configuration generators (e.g. Makefile generators or Ninja). <br /> Typical values include Debug, Release, RelWithDebInfo and MinSizeRel. <br /> More information [here](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). | Toolchain-specific (if value not set, Debug) |
| CMAKE_CONFIGURATION_TYPES | Build type on multi-configuration generators (e.g. Visual Studio, Xcode, or Ninja Multi-Config). <br /> Typical values include Debug, Release, RelWithDebInfo and MinSizeRel. <br /> More information [here](https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html). | Generator-specific (if value not set, Debug) |
| BUILD_TESTS | Build unit tests | OFF |
| BUILD_COVERAGE | Build with code coverage (for GCC only) | OFF |

The following commands can be utilized to configure the project (example for Debug configuration):

```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
```

To compile the software, use the CMake build command (universal command for building, that abstracts a native build tool's command-line interface):

```sh
$ cmake --build . -j 4
```

It is also possible to compile the project calling the underlying build system directly (example for Unix Makefiles):

```sh
$ make -j 4
```

## Running

After compiling the project, an executable file is created and can be run from the command line. It has the following command line options:

- `-V`, `--verbose`: enable verbose logs
- `-h`, `--help`: show help message
- `-i`, `--image`: image file path with the circuit
- `-s`, `--save-proc`: save images obtained during the processing in the working directory (the images with the regions of interest are always saved)
- `-v`, `--version`: show version

The `-i` or `--image` option is required to provide the image path of the circuit. So the software can be run with the following command (note that the executable may be located in a different directory, depending on the configuration generator), where `[OPTIONS]` are optional and can be one or more of the command line options previously described:

```sh
$ ./src/Debug/CircuitSegmentation -i <image_path> [OPTIONS]
```

## Tests

To run the unit tests, use the commands below (note that it is necessary to configure CMake with `BUILD_TESTS` option to ON):

```sh
$ cd <project-directory>
$ mkdir build-debug
$ cd build-debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
$ cmake --build . -j 4
$ ctest
```

## Documentation

The `docs` directory contains documentation related to the segmentation map generated by this software module, as well as some examples of results.

## Supported compilers

This project was successfully compiled using the following compilers (it might also compile using other):

| Compiler | Operating system |
| --- | --- |
| GCC 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) | Ubuntu 20.04.4 LTS (from docker image) |
| Microsoft Visual C++ 2022 / Build Tools 19.32.31332 | Windows 10 |

## Used third-party tools

This project uses some third-party tools for compiling, testing, code coverage, documentation, and code formatting and analysis.

### Build and test automation

[CMake](https://cmake.org/) is the system utilized to manage the build process of this project. [Docker](https://www.docker.com/) is the platform used to make it easy to build, run and test this software without installing the requirements locally.

### Code coverage

The [LCOV](https://github.com/linux-test-project/lcov) is the tool to obtain the code coverage. The project can be compiled for code coverage analysis, using the following commands (on Linux):

```sh
$ cd <project-directory>
$ mkdir build-coverage
$ cd build-coverage
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_COVERAGE=ON
$ cmake --build . -j 4
```

For generation of the report, use the correspondent script available in the `scripts` directory as follows:

```sh
$ cd <project-directory>
$ ./scripts/coverage-lcov-gen.sh build-coverage
```

The report file named `index.html` is created in the `build-coverage/coverage` directory.

### Doxygen documentation

The [doxygen](https://doxygen.nl/) tool is used to generate documentation from source code. The correspondent script generates the doxygen documentation, using the next commands:

```sh
$ cd <project-directory>
$ ./scripts/doxygen-doc-gen.sh ./doxygen/Doxyfile
```

The documentation is created and can be accessed in `build-doxygen/html/index.html`.

### Code formatting and analysis

This projects uses [clang-format](https://clang.llvm.org/docs/ClangFormat.html) tool for code formatting. The format of the code can be checked using the correspondent script:

```sh
$ cd <project-directory>
$ # To check the src directory
$ ./scripts/clang-format-check.sh ./src
```

For code static analysis, it is used the [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) tool. The correspondent script can be utilized to analyze the code:

```sh
$ cd <project-directory>
$ ./scripts/clang-tidy-check.sh
```

A report file is created in the `build-clang-tidy` directory.

## License

Licensed under the [MIT](./LICENSE) license @ [U=RIsolve](https://urisolve.pt/)
