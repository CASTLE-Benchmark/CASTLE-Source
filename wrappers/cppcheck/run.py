import os
import sys
import json
import timeit
import datetime


wrapper_container_tag = 'castle-wrapper-cppcheck'
tool_name = 'cppcheck'
execute_test_command = f'docker run --rm -v {os.path.join(os.getcwd(), "scan")}:/root/home/tests {wrapper_container_tag}'
dataset_file = '../../dataset.json'
testing_directory = './scan'
test_file_name = 'main.c'
report_file_name = 'report.txt'
final_report = f'../../reports/{tool_name}-report.json'
test_file_path = os.path.join(testing_directory, test_file_name)
report_file_path = os.path.join(testing_directory, report_file_name)
ignored_severities = [ 'information', 'style' ]


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

def parse_report(report: str):
    report = report.split('\n')
    findings = []
    for f in report:
        args = f.split(',')
        if len(args) == 4:
            if args[0] not in ignored_severities:
                findings.append({
                    "severity": args[0],
                    "line": int(args[1]),
                    "cwe": int(args[2]),
                    "message": args[3],
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
    with open(test_file_path, 'w') as f:
        f.write(t['code'])
    os.system(execute_test_command)
    
    # Gather report
    report = None
    with open(report_file_path, 'r') as f:
            report = f.read()
    if single_test:
        print(report)
        sys.exit(0)
    
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

# Clean up folder
for f in os.listdir(testing_directory):
    os.remove(os.path.join(testing_directory, f))
os.rmdir(testing_directory)
