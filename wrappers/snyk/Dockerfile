FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    curl \
    make

RUN curl --compressed https://downloads.snyk.io/cli/stable/snyk-linux -o snyk
RUN chmod +x ./snyk
RUN mv ./snyk /usr/local/bin/snyk

WORKDIR /root/home/tests

# Ignoring errors, bcs Snyk returns non-zero exit codes for vulnerabilities
ENTRYPOINT [ "make", "--ignore-errors" ] 