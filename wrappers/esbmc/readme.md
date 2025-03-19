# ESBMC

Version: 7.8.0

Sending the output to dev null, because it only contains metadata, the actual issues are sent to STDERR

```bash
esbmc --overflow --no-unwinding-assertions --unwind 1 --memory-leak-check --timeout 100 --multi-property --file-output /dev/null test.c 2> report.txt
```
