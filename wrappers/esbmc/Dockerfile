FROM ubuntu:24.04

# Install dependencies
RUN apt update
RUN apt install -y \
    clang-14 \
    llvm-14 \
    clang-tidy-14 \
    python-is-python3 \
    python3 \
    git \
    ccache \
    unzip \
    wget \
    curl \
    bison \
    flex \
    g++-multilib \
    linux-libc-dev \
    libboost-all-dev \
    libz3-dev \
    libclang-14-dev \
    libclang-cpp-dev \
    sqlite3 \
    libsqlite3-dev \
    libmysqlclient-dev \
    libcurl4-openssl-dev \
    openssl \
    cmake

# Compile ESBMC
WORKDIR /root/home
RUN git clone https://github.com/esbmc/esbmc.git && \
    mkdir build && cd build && \
    cmake ../esbmc -DENABLE_Z3=1 && \
    make -j4

RUN mv /root/home/build/src/esbmc/esbmc /usr/bin/esbmc
WORKDIR /root/home/tests

CMD [ "esbmc" ]