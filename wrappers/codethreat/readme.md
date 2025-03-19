# CodeThreat SAST

1. Log in with a GitHub account to CodeThreat: https://cloud.codethreat.com/
2. Create a GitHub repository with all the transpiled test files from `dataset.json`
3. On the site, Repositories > + > Repository Import, and import the repository
4. Wait for the scan to finish
5. Enter the python virtual environment `python -m venv .venv && source .venv/bin/activate`
6. Download the findings `python download.py`
7. Run the evaluator `python run.py`. Report will be deposited to the global reports folder
