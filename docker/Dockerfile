FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive

# Update repositories
RUN echo "Updating repositories..."
RUN apt-get update -y && apt-get upgrade -y

# Install certificates
RUN echo "Installing certificates..."
RUN apt-get install -y --no-install-recommends ca-certificates

# Install packages
RUN echo "Installing packages..."
RUN apt-get install -y --no-install-recommends \
    build-essential \
    clang-format \
    clang-tidy \
    cmake \
    doxygen \
    g++ \
    gcc \
    git \
    lcov \
    make \
    unzip \
    uuid \
    uuid-dev \
    wget

# Install OpenCV
RUN echo "Installing OpenCV library..."
RUN apt-get install -y --no-install-recommends libopencv-dev
# Download sources
# RUN wget https://github.com/opencv/opencv/archive/refs/tags/4.5.5.zip
# RUN unzip opencv-4.5.5.zip
# RUN cd opencv-4.5.5
# Configure and build
# RUN mkdir -p build && cd build
# RUN cmake ..
# RUN make -j 4
# Install
# RUN sudo make install
