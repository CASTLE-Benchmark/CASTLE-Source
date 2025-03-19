import os
import re
import sys
import json
import timeit
import datetime

import docker


tool_name = 'esbmc'
wrapper_container_tag = f'castle-wrapper-{tool_name}'
dataset_file = '../../dataset.json'
testing_directory = 'scan'
test_file_name = 'main.c'
test_timeout_seconds = 10
final_report = f'../../reports/{tool_name}-report.json'
internal_linux_workdir = '/root/home/tests'
test_file_path = os.path.join(testing_directory, test_file_name)
esbmc_command = f'esbmc --overflow --no-unwinding-assertions --memory-leak-check --timeout {test_timeout_seconds} --multi-property {test_file_name} --show-stacktrace'
docker_client = docker.from_env()

# esbmc_error_parser = r'(?P<file>.+?):(?P<line>\d+):(?P<column>\d+): (?P<severity>\w+): (?P<message>.+)'
esbmc_counterexample_marker = '[Counterexample]'
esbmc_violation_marker = 'Violated property:'
esbmc_stacktrace_marker = 'Stack trace:'
esbmc_violation_pattern = r'  file (?P<file>.*\.c) line (?P<line>\d+) column (?P<column>\d+) function (?P<function>[\w\d_.<>]+)'
esbmc_stacktrace_line_pattern = r'  c:@F@(?P<origin_function>[\w\d_.<>]+) at file (?P<file>.*\.c) line (?P<line>\d+) column (?P<column>\d+) function (?P<function>[\w\d_.<>]+)'
esbmc_stacktrace_end_marker = '  c:@F@main'

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
print(f"ESBMC command: {esbmc_command}")

def parse_direct_violation(report_lines: list[str], i: int, violation_dict: dict[str]) -> tuple[int, dict | None, str]:
    # Parsing violation, where the location is main.c
    n = len(report_lines)
    if i >= n or esbmc_stacktrace_marker not in report_lines[i]:
        return i, None, "Could not locate stacktrace for violated property"
    
    i += 1
    if violation_dict['function'] != 'main': # No stacktrace if the violation happend directly from main
        while i < n and esbmc_stacktrace_end_marker not in report_lines[i]:
            i += 1
        i += 1
    else:
        if i < n and esbmc_stacktrace_end_marker in report_lines[i]: # In some cases the end stacktrace is still present
            i += 1
    if i >= n:
        return i, None, "Could not locate message for violated property"
    
    msg = report_lines[i].strip()
    i += 1
    while i < n and len(report_lines[i].strip()) > 0:
        msg += ' ' + report_lines[i].strip()
        i += 1

    return i, {
        "severity": "error",
        "line": int(violation_dict['line']),
        "cwe": 0,
        "message": msg,
    }, "OK"

def parse_indirect_violation(report_lines: list[str], i: int) -> tuple[int, dict | None, str]:
    # Parsing violation, where the location is not main.c
    n = len(report_lines)
    if i >= n or esbmc_stacktrace_marker not in report_lines[i]:
        return i, None, "Could not locate stacktrace for violated property"
    
    i += 1
    top_main_func_call = None

    # Find first stacktrace line with main.c
    while i < n and esbmc_stacktrace_end_marker not in report_lines[i]:
        if top_main_func_call is None:
            stacktrace_match = re.match(esbmc_stacktrace_line_pattern, report_lines[i])
            if stacktrace_match:
                match_dict = stacktrace_match.groupdict()
                if match_dict['file'] == 'main.c':
                    top_main_func_call = match_dict
        i += 1
    if top_main_func_call is None:
        return i, None, "Could not locate main.c in stacktrace"
    
    i += 1
    if i >= n:
        return i, None, "Could not locate message for violated property"
    
    msg = report_lines[i].strip()
    i += 1
    while i < n and len(report_lines[i].strip()) > 0:
        msg += ' ' + report_lines[i].strip()
        i += 1

    return i, {
        "severity": "error",
        "line": int(top_main_func_call['line']),
        "cwe": 0,
        "message": msg,
    }, "OK"

def parse_counterexample(report_lines: list[str], i: int) -> tuple[int, dict | None, str]:
    n = len(report_lines)
    while i < n and esbmc_violation_marker not in report_lines[i]:
        i += 1
    i += 1
    if i >= n:
        return i, None, "Could not locate violated property for counterexample"
    
    violation_line = report_lines[i]
    violation_match = re.match(esbmc_violation_pattern, violation_line)
    if not violation_match:
        return i, None, "Violation line does not match the pattern"
    
    violation_dict = violation_match.groupdict()
    if violation_dict['file'] == 'main.c':
        return parse_direct_violation(report_lines, i + 1, violation_dict)
    return parse_indirect_violation(report_lines, i + 1)

def parse_report(report: str, test_id: str):
    findings = []
    report_lines = report.split('\n')
    i = 0
    while i < len(report_lines):
        if esbmc_counterexample_marker in report_lines[i]:
            i, result, error = parse_counterexample(report_lines, i + 1)
            if result is None:
                print(f'ERROR at {test_id}: Counterexample parsing error "{error}"')
            else:
                findings.append(result)
        i += 1

    if 'VERIFICATION FAILED' in report and len(findings) == 0:
        print(f'ERROR at {test_id}: Verification failed but no findings parsed')
        
    return findings

# Check run mode
single_test = False
if len(sys.argv) > 1:
    single_test = True
    test_id = int(sys.argv[1])
    tests = [ tests[test_id-1] ]

container = docker_client.containers.run(
    wrapper_container_tag,
    detach=True,
    volumes={
        os.path.join(os.getcwd(), testing_directory): {'bind': internal_linux_workdir, 'mode': 'rw'}
    },
    stdout=True,
    stderr=True,
    command='sleep infinity'
)

try:
    # Run tests
    reports = []
    start_time = timeit.default_timer()
    for i, t in enumerate(tests):
        ID = i+1
        print(f'[{ID}/{len(tests)}] {t["id"]} -> ', end='')
        test_time = timeit.default_timer()
            
        # Write code
        with open(test_file_path, 'w') as f:
            f.write(t['code'])
        
        # Run ESBMC
        result = container.exec_run(esbmc_command, stdout=True, stderr=True)
        
        # Get report
        report = result.output.decode('utf-8')
        findings = parse_report(report, t["id"])
        reports.append({
            "id": t["id"],
            "findings": findings,
            "report": report
        })
        
        if single_test:
            print(report)
            print(f'Findings: {findings}')
            sys.exit(0)

        print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')
finally:
    container.remove(force=True)

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
        "tool_type": "bmc",
        "tests": reports
    }, indent=4))
print(f'Report saved: {final_report}')
