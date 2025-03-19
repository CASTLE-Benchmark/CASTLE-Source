import os
import re
import json
import timeit
import datetime

wrapper_container_tag = 'castle-wrapper-snyk'
tool_name = 'snyk'
execute_test_command = f'docker run --rm -v {os.path.join(os.getcwd(), "scan")}:/root/home/tests {wrapper_container_tag} -e SNYK_TOKEN=\'{os.getenv("SNYK_TOKEN")}\''
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

start_time = timeit.default_timer()

# Create environment
if not os.path.exists(testing_directory):
    os.makedirs(testing_directory)
else:
    for f in os.listdir(testing_directory):
        os.remove(os.path.join(testing_directory, f))

print(f"Dataset: {dataset['dataset']}")
print(f"Version: {dataset['version']}")
print(f"Tests: ", len(tests))
print(f"Masked docker command: {execute_test_command.split('-e')[0]}")

# Creating test files
for test in tests:
    with open(os.path.join(testing_directory, test['name']), 'w') as f:
        f.write(test['code'])

# Running tool
make_file = os.path.join(testing_directory, 'Makefile')
with open(make_file, 'w') as f:
    f.write('all:\n')
    f.write(f'\t@snyk code test > report.txt\n')
os.system(execute_test_command)

def process_report_file(report_lines: list[str]) -> list[dict]:
    error_patterns = [
        r" ✗ \[(?P<type>[A-Z][a-z]*)\] (?P<title>.*) ",
        r"   Path: CASTLE-(?P<test_ind>\d+-\d+)\.c, line (?P<line>\d+) ",
        r"   Info: (?P<message>.*) "
    ]

    findings = []
    line_ind = 0
    while line_ind < len(report_lines):
        line = report_lines[line_ind]
        match = re.match(error_patterns[0], line)
        if not match:
            line_ind += 1
            continue

        components = match.groupdict()

        line = report_lines[line_ind + 1]
        match = re.match(error_patterns[1], line)
        if not match:
            line_ind += 1
            continue

        components.update(match.groupdict())

        line = report_lines[line_ind + 2]
        match = re.match(error_patterns[2], line)
        if not match:
            line_ind += 1
            continue

        components.update(match.groupdict())
        findings.append(components)
        line_ind += 3
    return findings

def format_finding(finding: dict) -> dict:
    return {
        'severity': finding['type'],
        'line': int(finding['line']),
        'cwe': 0,
        'message': finding['title'] + ': ' + finding['message']
    }

def format_report(report: dict, id: str) -> str:
    report_parts = report['message'].split(': ')
    title = report_parts[0]
    message = ': '.join(report_parts[1:])
    return f" ✗ \[{report['severity']}] {title} \n   Path: CASTLE-{id}.c, line {report['line']} \n   Info: {message} "

# Parsing report
report_lines = []
with open(report_file_path, 'r') as f:
    report_lines = f.readlines()

findings = process_report_file(report_lines)

reports = []
for test in tests:
    findings_for_test = [format_finding(f) for f in findings if f['test_ind'] == test['id']]
    reports.append({
        "id": test['id'],
        "findings": findings_for_test,
        "report": "\n\n".join([format_report(f, test['id']) for f in findings_for_test])
    })

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