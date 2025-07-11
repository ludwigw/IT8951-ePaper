#!/bin/sh
# Build and run the IT8951-ePaper test container
set -e

docker build -t it8951-epaper-test .
docker run --rm -it \
  -v "$(pwd)":/workspace \
  it8951-epaper-test 