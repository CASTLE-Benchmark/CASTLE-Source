FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    cppcheck

WORKDIR /root/home/tests

ENTRYPOINT [ "cppcheck", "--enable=all", "--language=c", "--output-file=./report.txt", "--quiet", "--template={severity},{line},{cwe},{message}" ]
CMD [ "main.c" ]
