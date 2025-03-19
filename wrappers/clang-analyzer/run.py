import os
import json
import timeit
import datetime
import re
import sys

import docker


wrapper_container_tag = 'castle-wrapper-clang-analyzer'
internal_linux_workdir = '/root/home/tests'
tool_name = 'clang-analyzer'
dataset_file = '../../dataset.json'
testing_directory = './scan'
final_report = f'../../reports/{tool_name}-report.json'

docker_client = docker.from_env()

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

def parse_report(report: str):
    vuln_line_regex = r"(?P<file>[\w\._-]+):(?P<line>\d+):(?P<column>\d+):\s*(?P<type>\w+):\s*(?P<message>.*)"
    
    report = report.split('\n')
    findings = []
    for line in report:
        match = re.match(vuln_line_regex, line)
        if not match:
            continue
        
        components = match.groupdict()
        findings.append({
            "severity": components['type'],
            "line": int(components['line']),
            "cwe": 0,
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
    container = None
    try:
        ID = i+1
        print(f'[{ID}/{len(tests)}] {t["id"]} -> ', end='')
        test_time = timeit.default_timer()
            
        # Write code
        code_file = os.path.join(testing_directory, t['name'])
        with open(code_file, 'w') as f:
            f.write(t['code'])
            
        # Write Makefile
        make_file = os.path.join(testing_directory, 'Makefile')
        with open(make_file, 'w') as f:
            f.write('all:\n')
            f.write(f'\t@{t["compile"]}\n')
            
        container = docker_client.containers.run(
            wrapper_container_tag,
            detach=True,
            volumes={os.path.join(os.getcwd(), "scan"): {'bind': internal_linux_workdir, 'mode': 'rw'}},
            environment=[],
            stdout=True,
            stderr=True,
        )
        # I tried without detaching and waiting but it was throwing a weird 404 error
        container.wait()
        
        # Gather report
        report = container.logs(stdout=True, stderr=True).decode('utf-8')
        if single_test:
            print(report)
            sys.exit(0)
        
        findings = parse_report(report)
        reports.append({
            "id": t["id"],
            "findings": findings,
            "report": report
        })
        
        # Clean up
        os.remove(code_file)
        os.remove(code_file[:-2]) # Output file
        os.remove(make_file)

        print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')
    finally:
        if container:
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
        "tests": reports
    }, indent=4))
print(f'Report saved: {final_report}')
