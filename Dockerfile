FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    sqlite3 \
    libsqlite3-dev \
    libmysqlclient-dev \
    perl \
    libcurl4-openssl-dev \
    openssl \
    gcc \
    make

WORKDIR /root/home/tests

ENTRYPOINT [ "/bin/bash" ]
