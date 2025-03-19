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
RUN apt-get install -y cbmc

WORKDIR /root/home/tests

CMD [ "cbmc" ]