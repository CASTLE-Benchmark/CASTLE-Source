# Calculate some statistics about the repository

import os
import json

# Get a list of all the files in the current directory and subdirectories
def get_files(root = '.'):
    blacklisted = ['__pycache__', '.git', '.vscode', '.venv']
    files = []
    for dir, dirs, filenames in os.walk(root):
        if any(blacklisted_dir in dir for blacklisted_dir in blacklisted):
            continue
        for filename in filenames:
            files.append(os.path.join(dir, filename))
    return files

# Get the number of lines in all python files
def get_python_lines(files) -> int:
    lines = 0
    for file in files:
        if file.endswith('.py'):
            with open(file, 'r') as f:
                lines += len(f.readlines())
    return lines

# Get the number of code lines in all ipynb files
def get_ipynb_lines(files) -> int:
    lines = 0
    for file in files:
        if file.endswith('.ipynb'):
            with open(file, 'r') as f:
                notebook = json.load(f)
                for cell in notebook['cells']:
                    if cell['cell_type'] == 'code':
                        lines += len(cell['source'])
    return lines

# Get the number of findings in all the reports
def get_findings() -> int:
    findings = 0
    for file in files:
        if file.startswith('./reports/') and file.endswith('-report.json'):
            with open(file, 'r') as f:
                report = json.load(f)
                tests = report['tests']
                for test in tests:
                    findings += len(test['findings'])
    return findings

files = get_files()
print(f'Python code length: {get_python_lines(files)+get_ipynb_lines(files)}')
print(f'Number Python scripts: {len([file for file in files if file.endswith(".py") or file.endswith(".ipynb")])}')
print(f'Number of Docker containers: {len([file for file in files if 'Dockerfile' in file])}')
print(f'Number of findings manually validated: {get_findings()}')