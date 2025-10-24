# Dockerfile for SAI Virtual Switch Development Environment
FROM ubuntu:22.04

# Avoid interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    autoconf \
    automake \
    libtool \
    pkg-config \
    libzmq5-dev \
    libboost-dev \
    libnl-3-dev \
    libnl-route-3-dev \
    libnl-nf-3-dev \
    libnl-genl-3-dev \
    libhiredis-dev \
    swig \
    libgtest-dev \
    nlohmann-json3-dev \
    iproute2 \
    iputils-ping \
    tcpdump \
    net-tools \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Create workspace
WORKDIR /workspace

# Clone sonic-swss-common (required dependency)
RUN git clone https://github.com/sonic-net/sonic-swss-common.git && \
    cd sonic-swss-common && \
    git checkout 202411 && \
    ./autogen.sh && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    ldconfig

# Clone sonic-sairedis (contains virtual switch)
RUN git clone https://github.com/sonic-net/sonic-sairedis.git && \
    cd sonic-sairedis && \
    git checkout 202411 && \
    git submodule update --init --recursive

# Build sonic-sairedis
RUN cd sonic-sairedis && \
    ./autogen.sh && \
    ./configure --with-sai=vs && \
    make -j$(nproc) && \
    make install && \
    ldconfig

# Copy application source
COPY src/ /workspace/app/
COPY Makefile /workspace/app/

# Set working directory for the application
WORKDIR /workspace/app

# Build the example application
RUN make clean && make

# Default command
CMD ["/bin/bash"]
