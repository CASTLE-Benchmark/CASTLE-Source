# Print the findings from a report one by one for manual evaluation

import json
import sys
import os
import re

reports_directory = 'reports'
report_file_postfix = '-report.json'
dataset_file = 'dataset.json'

# Load parameters
tool = None
id = None
if len(sys.argv) > 2:
    tool = sys.argv[1]
    id = sys.argv[2]
else:
    print(f"Usage: python3 {sys.argv[0]} <tool> <id>")
    print(f"   tool: codeql, esbmc, gpt-4o, ...")
    print(f"   id: <cwe>-<number>, 22-3")
    sys.exit(1)
report_path = os.path.join(reports_directory, tool + report_file_postfix)

# Load report
if not os.path.exists(report_path):
    print(f"Report not found: {report_path}")
    sys.exit(1)
with open(report_path, 'r') as f:
    reports = json.loads(f.read())
tests = reports['tests']

# Find report
report = None
for t in tests:
    if t['id'] == id:
        report = t
        break
if report is None:
    print(f"Report not found for {tool} and {id}")
    sys.exit(1)

# Load dataset
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
dataset_tests = dataset['tests']
dataset_test = None
for t in dataset_tests:
    if t['id'] == id:
        dataset_test = t
        break
if dataset_test is None:
    print(f"Report {id} not found in dataset!")
    sys.exit(1)

def find_line_number(index: int, line_ends: list[int]):
    for i, line_end in enumerate(line_ends):
        if index < line_end:
            return i + 1
    return None

def get_file_deleted_line_ranges(code_file: str) -> list[tuple[int, int]]:
    with open(code_file, 'r') as f:
        code = f.read()

    line_lengths = [len(l) for l in code.splitlines(True)]
    line_ends = [sum(line_lengths[:i+1]) for i in range(len(line_lengths))]

    re_multiline_comment = r'/\*(.*?)\*/'
    matches = re.finditer(re_multiline_comment, code, re.DOTALL)

    deleted_line_ranges = []
    for match in matches:
        start_line = find_line_number(match.start(), line_ends)
        end_line = find_line_number(match.end() - 1, line_ends)
        if start_line is not None and start_line != end_line:
            deleted_line_ranges.append((start_line, end_line))
    return deleted_line_ranges[1:] # The first match is always the header (1, 12)

def get_line_in_original_file(test_id: str, line: int) -> int:
    cwe = test_id.split('-')[0]
    deleted_line_ranges = get_file_deleted_line_ranges(f'dataset/{cwe}/CASTLE-{test_id}.c')
    line += 13 # Header offset
    for start, end in deleted_line_ranges:
        if line >= start:
            line += end - start
    return line

def shorten_codethreat_finding(msg: str) -> str:
    if 'The program copies an input buffer to an output buffer without verifying that the size of the input buffer is less than the size of the output buffer, leading to a buffer overflow' in msg:
        return "Buffer copy without size verification"
    elif 'The software uses a function that accepts a format string as an argument, but the format string originates from an external source' in msg:
        return "Format string from external source"
    elif 'The program contains a code sequence that can run concurrently with other code, and the code sequence requires temporary, exclusive access to a shared resource, but a timing window exists in which the shared resource can be modified by another code sequence that is operating concurrently' in msg:
        return "Concurrent resource access"
    elif 'The product receives input or data, but it does not validate or incorrectly validates that the input has the properties that are required to process the data safely and correctly' in msg:
        return "Missing or incorrect input validation"
    return msg

# Print report
print(f"Tool: {tool}, ID: {id}\n")
print(f"CWE-{dataset_test['cwe']}: {dataset_test['description']}")
if len(dataset_test['lines']) > 0:
    print(f"Lines: {', '.join([str(get_line_in_original_file(id, l)) for l in dataset_test['lines']])}")
else:
    print("Non-vulnerable")
print()
if len(report['findings']) == 0:
    print('No findings')
for finding in report['findings']:
    original_line = get_line_in_original_file(id, finding['line'])
    msg = shorten_codethreat_finding(finding['message']) if tool == 'codethreat' else finding['message']
    print(f"{original_line}: ({finding['severity']}) {msg}")
    if not msg.endswith('\n'):
        print()