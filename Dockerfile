# IT8951-ePaper Dockerfile for development and testing

FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && \
    apt-get install -y build-essential libbcm2835-dev make git imagemagick && \
    rm -rf /var/lib/apt/lists/*

# Set workdir
WORKDIR /workspace

# Copy everything
COPY . .

# Build library, CLI, and tests
RUN make clean && make -C tests clean && make bin/epdraw PLATFORM=BCM && make test

# Default command: run all tests
CMD ["make", "-C", "tests", "run"] 