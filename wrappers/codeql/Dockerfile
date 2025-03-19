FROM castle-builder-ubuntu:latest

ENV CODEQL_VERSION=3.28.1
RUN apt install -y curl unzip git

# Install CodeQL CLI
WORKDIR /root/home
RUN curl -L https://github.com/github/codeql-cli-binaries/releases/download/v2.20.1/codeql-linux64.zip -o codeql.zip && \
    unzip codeql.zip && \
    mv codeql /opt/codeql && \
    rm codeql.zip
ENV PATH="/opt/codeql:${PATH}"

# Install codeql tests
RUN git clone --depth 1 --recursive https://github.com/github/codeql.git codeql-repo

WORKDIR /root/home/tests
