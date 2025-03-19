import os
import sys
import json
import timeit
import datetime

import docker


# Burnt in runner for now
wrapper_container_tag = 'semgrep/semgrep:pro-sha-5030a8e'
tool_name = 'semgrep-code'
config_file = 'config.json'
config_token_key = 'semgrep_token'
internal_linux_workdir = '/src'
dataset_file = '../../dataset.json'
testing_directory = './scan'
final_report = f'../../reports/{tool_name}-report.json'
semgrep_token = None
docker_client = docker.from_env()
internal_report_file_name = 'report.json'
semgrep_command = f'semgrep scan --include=`FILENAME` --json --json-output={internal_report_file_name} --secrets --config auto --quiet --pro'

# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

# Load config
with open(config_file, 'r') as f:
    config = json.loads(f.read())
    semgrep_token = config[config_token_key]

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
    # Example
    # !!AUTO REDACTED!!
    parsed = json.loads(report)
    findings = []
    for w in parsed['results']:
        findings.append({
            "severity": w['extra']['severity'].lower(),
            "line": int(w['start']['line']),
            "cwe": int(w['extra']['metadata']['cwe'][0].split(':')[0].split('-')[1]),
            "message": w['extra']['message'],
        })
    return findings

# Check run mode
single_test = False
if len(sys.argv) > 1:
    single_test = True
    test_id = int(sys.argv[1])
    tests = [ tests[test_id-1] ]

print(f"Sarting container...")
container = docker_client.containers.run(
    wrapper_container_tag,
    detach=True,
    volumes={
        os.path.join(os.getcwd(), "scan"): {'bind': internal_linux_workdir, 'mode': 'rw'}
    },
    environment={
        'SEMGREP_APP_TOKEN': semgrep_token
    },
    stdout=True,
    stderr=True,
    command='sleep infinity'
)
print(f"Container ID: {container.id}")

# Run tests
reports = []
start_time = timeit.default_timer()
for i, t in enumerate(tests):
    try:
        ID = i+1
        print(f'[{ID}/{len(tests)}] {t["id"]} -> ', end='')
        test_time = timeit.default_timer()
            
        # Write code
        code_file = os.path.join(testing_directory, t['name'])
        with open(code_file, 'w') as f:
            f.write(t['code'])
        
        container.exec_run(semgrep_command.replace('`FILENAME`', t['name']), stdout=True, stderr=True)
        
        # Gather report
        report = None
        with open(os.path.join(testing_directory, internal_report_file_name), 'r') as f:
            report = f.read()
        if single_test:
            print(json.dumps(json.loads(report), indent=4))
            sys.exit(0)
        
        findings = parse_report(report)
        reports.append({
            "id": t["id"],
            "findings": findings,
            "report": report
        })

        # Clean up
        os.remove(code_file)
        
        print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')
    except Exception as e:
        print(f'Error: {e}')
        raise e

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