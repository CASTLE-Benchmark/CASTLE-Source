FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    splint \
    sqlite3 \
    libsqlite3-dev \
    libmysqlclient-dev \
    perl \
    clang \
    libcurl4-openssl-dev \
    openssl \
    clang-tools \
    gcc \
    make

WORKDIR /root/home/tests

# Ignoring errors, bcs splint returns non-zero exit codes for warnings
ENTRYPOINT [ "make", "--ignore-errors" ] 