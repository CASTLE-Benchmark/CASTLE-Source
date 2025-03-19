# Print the result of a specific test from a report

import json
import sys
import os

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
print(f"Loading report from {report_path}")

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

# Print report
print(f"Report for {tool} and {id}")
print(f"Expected:")
print(f"   vulnerable: { 'Yes' if dataset_test['vulnerable'] else 'No' }")
print(f"   lines: { ', '.join([str(l) for l in dataset_test['lines'] ]) }")
print(f"   cwe: {dataset_test['cwe']}")
print(f"   message: {dataset_test['description']}")
print(f"Findings:")
for i,f in enumerate(report['findings']):
    print(f"### {i+1}:")
    print(f"   severity: {f['severity']}")
    print(f"   line: {f['line']}")
    print(f"   cwe: {f['cwe']}")
    print(f"   message: {f['message']}")
print(f"Output:")
print(f"{report['report']}")