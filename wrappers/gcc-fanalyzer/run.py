import os
import re
import sys
import json
import timeit
import datetime


wrapper_container_tag = 'castle-wrapper-gcc-fanalyzer'
tool_name = 'gcc-fanalyzer'
execute_test_command = f'docker run --rm -v {os.path.join(os.getcwd(), "scan")}:/root/home/tests {wrapper_container_tag}'
dataset_file = '../../dataset.json'
testing_directory = './scan'
report_file_name = 'report.txt'
final_report = f'../../reports/{tool_name}-report.json'
report_file_path = os.path.join(testing_directory, report_file_name)


# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

# Create environment
if not os.path.exists(testing_directory):
    os.makedirs(testing_directory)
else:
    for f in os.listdir(testing_directory):
        os.remove(os.path.join(testing_directory, f))

print(f"Dataset: {dataset['dataset']}")
print(f"Version: {dataset['version']}")
print(f"Tests: ", len(tests))
print(f"Docker command: {execute_test_command}")

def get_cwe(message: str):
    pattern = r"\[CWE-(\d+)\]"
    match = re.search(pattern, message)
    return int(match.group(1)) if match else 0

def parse_report(report: str):
    report_lines = report.split('\n')
    pattern = r"(?P<file>[\w\._-]+):(?P<line>\d+):(?P<column>\d+):\s*(?P<type>\w+):\s*(?P<message>.*)"
    findings = []
    for line in report_lines:
        match = re.match(pattern, line)
        if not match:
            continue

        components = match.groupdict()
        findings.append({
            "severity": components['type'],
            "line": int(components['line']),
            "cwe": get_cwe(components['message']),
            "message": components['message'],
        })
    return findings

# Check run mode
single_test = False
if len(sys.argv) > 1:
    single_test = True
    test_id = int(sys.argv[1])
    tests = [ tests[test_id-1] ]

# Run tests
reports = []
start_time = timeit.default_timer()
for i, t in enumerate(tests):
    ID = i+1
    print(f'[{ID}/{len(tests)}] {t["id"]} -> ', end='')
    test_time = timeit.default_timer()
        
    # Execute test
    with open(os.path.join(testing_directory, t['name']), 'w') as f:
        f.write(t['code'])

    make_file = os.path.join(testing_directory, 'Makefile')
    with open(make_file, 'w') as f:
        f.write('all:\n')
        f.write(f'\t@{t["compile"]} -Wall -Wextra -fanalyzer 2> report.txt\n')
    
    os.system(execute_test_command)
    
    # Gather report
    report = None
    with open(report_file_path, 'r') as f:
            report = f.read()
    if single_test:
        print(report)
        #sys.exit(0)
    
    findings = parse_report(report)
    reports.append({
        "id": t['id'],
        "findings": findings,
        "report": report
    })

    print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')

stop_time = timeit.default_timer()
runtime = stop_time - start_time
print(f'Runtime: {runtime:.2f}s')
print(f'Runtime avg per test: {(runtime/len(tests)):.2f}s')

# Export report
with open(final_report, 'w') as f:
    f.write(json.dumps({
        "dataset": dataset['dataset'],
        "version": dataset['version'],
        "tool": tool_name,
        "date": datetime.datetime.now().isoformat(),
        "runtime": runtime,
        "tests": reports
    }, indent=4))
print(f'Report saved: {final_report}')
