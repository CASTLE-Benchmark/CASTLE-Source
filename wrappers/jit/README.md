# Jit

https://platform.jit.io/

A SAST, which integrates to GitHub, and checks PR-s. It uses Semgrep with some extra filters for C/C++ code. Apperently these filters seem to filter out all semgrep reports, since the semgrep-related action passes

## Prerequisites

- Create and empty **GitHub** repo
- Create a PR adding all the test
    - **IMPORTANT**: Do not add the test under any of the following folders: `spec`, `test`, `tests`, `tmp`
- Authorize Jit on your repo
- Wait until Jit's actions run
- Open the pipeline result on Jit's site
    - **IMPORTANT**: Jit only comments the top 10 issues to the PR, so checking Jit's site is necessary
- Download the logs of the run jobs and paste the to the `log` folder here