docker build --platform linux/amd64 -t castle-wrapper-cbmc .

cbmc --cover-failed-assertions --localize-faults --gcc --json-ui --unwinding-assertions --unwind 10 787/CASTLE-787-2.c
