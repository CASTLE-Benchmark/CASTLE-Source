## Coverity

Running this tool is quite involved and requires a full local setup as well as access to a prepaid plan.

## Setup

1. Log into the dashboard, create a token and save the token into `.token` file
2. Download the `polaris` executable from the platform
3. Go to the CASTLE Repository folder with all the tests and the Makefile. A Makefile must be present with the full linker parameters for mysql-client, sqlite, openssl, curl. Since the other tools don't need it I created it manually with a replace. Example:
```bash
CASTLE-78-4: CASTLE-78-4.c
	gcc CASTLE-78-4.c -o CASTLE-78-4 -I/opt/homebrew/opt/curl/include -I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/mysql-client/include -I/opt/homebrew/opt/sqlite/include -L/opt/homebrew/opt/curl/lib -L/opt/homebrew/opt/openssl@3/lib -L/opt/homebrew/opt/mysql-client/lib -L/opt/homebrew/opt/sqlite/lib
```
4. Run `polaris setup`
5. Run `polaris capture`
6. Run `polaris analyze`
7. Wait for the scan to complete and download the CSV with: Issues > Export All Matching > CSV > Export
8. Place the CSV into the coverity wrapper directory with the name: `coverity-report.csv`
9. Run the evaluation: `process.py`