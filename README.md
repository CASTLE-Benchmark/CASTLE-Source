# CASTLE Source

This is the source repository for the CASTLE Benchmark project. This repository is a cleaned version of our internal repository to avoid leaking sensitive and copyrighted information. For more information on the project please look at https://github.com/CASTLE-Benchmark.

Authors: _Richard A. Dubniczky, Krisztofer Zoltan Horvát, Tamás Bisztray, Mohamed Amine Ferrag, Lucas C. Cordeiro, and Norbert Tihanyi_

## Repository Statistics

- Lines of Python code for parsing, wrappers and evaluation: `5284`
- Number of Python scripts: `34`
- Lines of C code for the tests: `10521`
- Number of custom Docker containers: `11`
- Number of commits to the project: `374`

## Wrappers

Wrappers are the way we automate as much of the evaluation process of a tool as possible. Due to the various licensing models, some tools are as easy as just running them in a container and gathering the reports, while others require registering and uploading code or linking repositories, and later getting the results using APIs or summary downloads.

All the tested tools must:
- Provide a report that we can export going through all findings manually
- Provide at least a limited free trial without the requirement to contact sales
- Be able to scan C and/or C++ projects

|Tool|Type|Credentials|Notes|
|-|-|-|-|
|[Aikido](./wrappers/aikido/)|`web`, `download`|Yes|Needs registering, linking a repository, then downloading the result CSV and loading it into the evaluator|
|[CBMC](./wrappers/cbmc/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[Clang Analyzer](./wrappers/clang-analyzer/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[CodeQL](./wrappers/codeql/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[Code Threat](./wrappers/codethreat/)|`web`, `api`|Yes|Needs registering, linking a repository, then the wrapper will download and evaluate the results automatically via an API|
|[Coverity](./wrappers/coverity/)|`web`|Yes|Upload files with a local container and manually view on the website|
|[CppCheck](./wrappers/cppcheck/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[ESBMC](./wrappers/esbmc/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[GCC Fanalyzer](./wrappers/gcc-fanalyzer/)|`container`|No|Simple CLI and build tools wrapped in a container|
|[Gitlab SAST](./wrappers/gcc-fanalyzer/)|`web`, `download`|Yes|Needs a gitlub project with the code committed and the result of the scan downloaded|
|[Jit](./wrappers/jit/)|`web`, `download`|Yes|Needs registering, linking a repository, then downloading the result and loading it into the evaluator|
|[Semgrep](./wrappers/semgrep/)|`container`|Yes|Needs a token to work with a registered user, but otherwise works from the CLI|
|[Snyk](./wrappers/snyk/)|`api`|Yes|Needs registering and a token, and uses API requests for each check with a daily limit|
|[SonarQube](./wrappers/snyk/)|`web`, `api`|Yes|Needs registering, linking a repository, then the wrapper will download and evaluate the results automatically via an API|
|[Splint](./wrappers/splint/)|`container`|Yes|Simple CLI and build tools wrapped in a container|
|[LLM](/wrappers/llm)|`api`|Yes|API requests with a token with a pay-per-request model with a generic OpenAI API compliant interface|

## Steps

### 1. Generate the Dataset JSON

Takes the `dataset` folder and creates a `dataset.json`. The JSON contains all information for running the tests, and some metadata for classifying the results.

```bash
python -m venv .venv
. ./.venv/bin/activate
pip install -r requirements.txt
python parser.py
```

### 2. Select a test wrapper and run it

Running a test wrapper will usually use the `scan` folder in it's directory for performing the scans. In all cases you need to have the dataset built and inside the root of the repository. Please check out each wrapper for more information and additional scripts, as they all have a readme attached.

For most wrappers, you need to have docker installed on your system and have the docker daemon running.

> After running multiple tests especially, it's advised to look at your local image repository in docker, as many of the containers are huge.

Please open the runner python file, as generally it will have settings that need to be updated, such as software versions.

For example with `cppcheck`:

```bash
cd wrappers/cppcheck
./build.sh # Build the container
python run.py
```

After generating a report, it is saved into the reports directory. Please commit it! We want to have a traceable timeline of reports for reference to older ones if possible.

### 3. Creating the statistics

The statistics are generated in the [charts.ipynb](./charts.ipynb) Jupyter Notebook. In case you are using VSC, you can install an extension and view/run it in your IDE. The notebook will load the `dataset.json`, the `cwe-collection.yaml`, as well as all the reports to generate the statistics and graphs.

It is recommended that you use the locally created python virtual environment from Step 1, so you won't have to install them globally.

## Sources

- https://samate.nist.gov/SARD/test-cases/
- https://cwe.mitre.org/index.html
- https://cwe.mitre.org/top25/archive/2024/2024_cwe_top25.html
- https://cwe.mitre.org/top25/archive/2023/2023_top25_list.html#tableView
- https://en.wikipedia.org/wiki/List_of_tools_for_static_code_analysis
