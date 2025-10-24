FROM debian:bookworm

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Clone SAI headers from OCP repository
RUN git clone --depth 1 https://github.com/opencomputeproject/SAI.git /tmp/SAI && \
    mkdir -p /usr/include/sai && \
    cp /tmp/SAI/inc/*.h /usr/include/sai/ && \
    cp /tmp/SAI/experimental/*.h /usr/include/sai/ 2>/dev/null || true && \
    rm -rf /tmp/SAI

# Copy and install Broadcom SAI package
COPY debs/*.deb /tmp/
RUN dpkg -i /tmp/*.deb || true && \
    apt-get update && \
    apt-get install -f -y && \
    rm -rf /tmp/*.deb && \
    ln -sf /usr/lib/libsai.so.1 /usr/lib/libsai.so && \
    ldconfig

# Copy source code
WORKDIR /workspace
COPY src/ .

# Build application
RUN make

CMD ["/bin/bash"]
