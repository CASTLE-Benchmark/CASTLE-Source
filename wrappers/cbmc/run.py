import os
import re
import sys
import json
import timeit
import datetime

import docker


tool_name = 'cbmc'
wrapper_container_tag = f'castle-wrapper-{tool_name}'
dataset_file = '../../dataset.json'
testing_directory = 'scan'
test_file_name = 'main.c'
final_report = f'../../reports/{tool_name}-report.json'
internal_linux_workdir = '/root/home/tests'
test_file_path = os.path.join(testing_directory, test_file_name)
docker_client = docker.from_env()
cbmc_max_unwind = 10
command = f'cbmc --cover-failed-assertions --localize-faults --gcc --json-ui --no-unwinding-assertions --unwind {cbmc_max_unwind} {test_file_name}'


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
print(f"{tool_name} command: {command}")

def parse_report(report):
    findings = []
    for entry in report:
        if 'result' in entry:
            for result in entry['result']:
                if result.get('status') == 'FAILURE':
                    # Extract basic information
                    description = result.get('description', 'No description available')
                    severity = 'high'  # All failures considered High severity based on example
                    
                    # Find relevant line number in user's code
                    line_number = 0
                    
                    # Check trace steps for function call in user's code
                    for step in result.get('trace', []):
                        if step.get('stepType') == 'function-call':
                            sl = step.get('sourceLocation', {})
                            if sl.get('file', '').endswith('CASTLE-787-2.c'):
                                line_number = sl.get('line', 0)
                                break
                    
                    # Fallback to failure's source location if not found in trace
                    if line_number == 0:
                        sl = result.get('sourceLocation', {})
                        line_number = sl.get('line', 0)
                    
                    findings.append({
                        'severity': severity,
                        'line': int(line_number),
                        'cwe': 0,
                        'message': description,
                    })
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
print(f'Wrapper container started: {container.id}')

invalid_jsons = []
try:
    # Run tests
    reports = []
    start_time = timeit.default_timer()
    for i, t in enumerate(tests):
        ID = i+1
        print(f'[{ID}/{len(tests)}] {t["id"]} -> ', end='', flush=True)
        test_time = timeit.default_timer()
            
        # Write code
        with open(test_file_path, 'w') as f:
            f.write(t['code'])
        
        # Run CBMC
        result = container.exec_run(command, stdout=True, stderr=True)
        
        # Get report
        report = result.output.decode('utf-8')
        try:
            json_report = json.loads(report)
            findings = parse_report(json_report)
            reports.append({
                "id": t["id"],
                "findings": findings,
                "report": report
            })
        except json.JSONDecodeError:
            invalid_jsons.append(t["id"])
            print(f'!Invalid')
            reports.append({
                "id": t["id"],
                "findings": [],
                "report": report
            })
        
        if single_test:
            print(report)
            print(f'Findings: {findings}')
            sys.exit(0)

        print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')
finally:
    container.remove(force=True)
    
if len(invalid_jsons) > 0:
    print(f'WARNING: Invalid reports: {len(invalid_jsons)}')
    print(invalid_jsons)

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
