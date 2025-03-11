FROM ubuntu:latest

# Install necessary tools for epoll development
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    clang \
    vim \
    git \
    man \
    strace \
    ltrace \
    lsof

# Clean up unnecessary files to reduce image size
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
