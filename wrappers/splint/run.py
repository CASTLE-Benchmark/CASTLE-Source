import os
import re
import sys
import json
import timeit
import datetime


wrapper_container_tag = 'castle-wrapper-splint'
tool_name = 'splint'
execute_test_command = f'docker run --rm -v {os.path.join(os.getcwd(), "scan")}:/root/home/tests {wrapper_container_tag}'
dataset_file = '../../dataset.json'
testing_directory = './scan'
report_file_name = 'report.txt'
final_report = f'../../reports/{tool_name}-report.json'
report_file_path = os.path.join(testing_directory, report_file_name)
splint_flags = [
    '+trytorecover', # Try to recover from parse errors
    '+posixlib', # Use POSIX libraries
    '-preproc', # Skip checking header files
    '-exportlocal', # Skip warning for non-static free functions

    # Other flags which signaled no real error
    # '-exitarg',
    # '-retvalint', 
    # '-paramuse',
    # '-retvalother',
    # '+matchanyintegral',
    # '-predboolint',
    # '+ignoresigns',
    # '+charintliteral',
    # '-unrecog',
    # '-incompletetype',
    # '+charint',
    # '-compdef',
    # '+boolint',
    # '-boolops',
    # '-nullret',
    # '-nullpass',
    # '-usedef',
    # '+relaxtypes',
    # '-realcompare',
    # '-initallelements',
    # '-branchstate',
]


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

def remove_last_line(text: str):
    return '\n'.join(text.split('\n')[:-1])

def parse_report(report: str, test_name: str):
    trimmed_report = '\n'.join([line.strip() for line in report.split('\n')[2:-2]])
    report_sections = [remove_last_line(section) for section in trimmed_report.split(test_name)]
    pattern = r":(?P<line>\d+):(?P<column>\d+): (?P<message>.*)"
    findings = []
    for section in report_sections:
        if 'Parse Error' in section or 'Cannot recover from parse error' in section:
            continue
        section_lines = section.split('\n')
        match = re.match(pattern, section_lines[0])
        if not match:
            continue

        components = match.groupdict()
        findings.append({
            "severity": 'warning',
            "line": int(components['line']),
            "cwe": 0,
            "message": components['message'] + '\n' + '\n'.join(section_lines[1:]),
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
splint_flags_str = ' '.join(splint_flags)
start_time = timeit.default_timer()
for i, t in enumerate(tests):
    ID = i+1
    print(f'[{ID}/{len(tests)}] TEST-{ID} -> ', end='')
    test_time = timeit.default_timer()
        
    # Execute test
    with open(os.path.join(testing_directory, t['name']), 'w') as f:
        f.write(t['code'])

    make_file = os.path.join(testing_directory, 'Makefile')
    with open(make_file, 'w') as f:
        f.write('all:\n')
        f.write(f'\t@splint {t["name"]} {splint_flags_str} 1> report.txt 2>&1\n')
    
    os.system(execute_test_command)
    
    # Gather report
    report = None
    with open(report_file_path, 'r') as f:
        report = f.read()
    if single_test:
        print(report)
        #sys.exit(0)
    
    findings = parse_report(report, t["name"])
    reports.append({
        "id": t["id"],
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
