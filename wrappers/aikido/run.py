import csv
import json
import datetime


tool_name = 'aikido'
report_file = 'report.json'
dataset_file = '../../dataset.json'
repository_name = 'castle'
finding_types = [ 'sast', 'leaked_secret' ]
tool_version = ''
final_report = f'../../reports/{tool_name}-report.json'


# Read data
issues : list[dict[str,str]] = []
with open('issues.csv', 'r', newline='') as f:
    reader = csv.reader(f)
    for line in reader:
        issues += [line]
headers = issues[0]
issues = issues[1:]

with open(dataset_file, 'r') as f:
    dataset = json.load(f)
dataset_tests = dataset['tests']

print(f"Dataset: {dataset['dataset']}")
print(f"Version: {dataset['version']}")
print(f"Tests: ", len(dataset_tests))
print(f"Tool: {tool_name} {tool_version}")
print(f"Finding count: {len(issues)}")

# Convert from CSV to list of dictionaries
processed = []
for issue in issues:
    p = {}
    for i in range(len(headers)):
        p[ headers[i] ] = issue[i].strip()
    
    # Parse specific fields
    p['start_line'] = int(p['start_line'])
    p['end_line'] = int(p['end_line'])
    p['time_to_fix_minutes'] = int(p['time_to_fix_minutes'])
    
    processed.append(p)
print(f"Preprocessed findings: {len(processed)}")

# Filter findings
processed = list(filter(lambda x: x['code_repo'] == repository_name, processed)) # Ignore findings from other repositories in case it was imported incorrectly
processed = list(filter(lambda x: x['code_repo_branch'] == 'main' or x['code_repo_branch'] == '', processed)) # Make sure there are no duplicated findings because of multiple branches, some findings contain no branch for some reason
processed = list(filter(lambda x: x['type'] in finding_types, processed)) # We are only interested in specific finding types

print(f"Filtered irrelevant findings: {len(processed)} left")

def file_to_id(file: str) -> str:
    file = file.split('/')[-1] # Remove path
    file = file.split('.')[0] # Remove extension
    return file.removeprefix('CASTLE-')

# Assign findings to tests
results = {}
for t in dataset_tests:
    results[t['id']] = []
    
for p in processed:
    results[ file_to_id(p['affected_file']) ].append(p)
    

# Create report
test_list = []
for i,t in enumerate(results):
    ID = i+1
    
    findings = []
    for finding in results[t]:
        findings.append({
            "severity": finding['severity'],
            "line": finding['start_line'],
            "cwe": '',
            "message": finding['rule'], # Not an actual description, but the rule name summarizes the finding well enough
        })
    
    report = json.dumps(results[t]) if len(findings) > 0 else ''
    test_list.append({
        "id": t,
        "findings": findings,
        "report": report
    })
    
# Export report
with open(final_report, 'w') as f:
    f.write(json.dumps({
        "dataset": dataset['dataset'],
        "version": dataset['version'],
        "tool": tool_name,
        "tool_version": tool_version,
        "date": datetime.datetime.now().isoformat(),
        "runtime": 0,
        "tests": test_list
    }, indent=4))
print(f'Report saved: {final_report}')