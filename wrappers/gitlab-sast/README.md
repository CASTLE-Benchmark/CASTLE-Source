# Gitlab SAST

Gitlab has a built-in SAST job for commits. It uses semgrep for C/C++ codes with custom filters

## Prerequisites

- Create and empty **Gitlab** repo with SAST initialized (a possible parameter, when creating repo online)
- Create a commit adding all the test
    - **IMPORTANT**: Do not add the test under any of the following folders: `spec`, `test`, `tests`, `tmp`
- Wait until SAST actions run
- Download the `gl-sast-report.json` artifact and paste it into this folder