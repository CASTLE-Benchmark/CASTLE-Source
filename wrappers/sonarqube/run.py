import json
import datetime


tool_name = 'sonarqube'
report_file = 'sonarqube.json'
organization_name = 'castle-benchmark'
repository_name = 'Tests-C'
dataset_file = '../../dataset.json'
tool_version = ''
final_report = f'../../reports/{tool_name}-report.json'

sq_project = f'{organization_name}_{repository_name}'
# jq -r '.[].severity' sonarqube.json | sort | uniq
severity_mapping = {
    'MINOR': 'low',
    'MAJOR': 'medium',
    'CRITICAL': 'high',
    'BLOCKER': 'critical',
}


# Read data
with open(report_file, 'r') as f:
    issues = json.load(f)

with open(dataset_file, 'r') as f:
    dataset = json.load(f)
dataset_tests = dataset['tests']

print(f"Dataset: {dataset['dataset']}")
print(f"Version: {dataset['version']}")
print(f"Tests: ", len(dataset_tests))
print(f"Tool: {tool_name} {tool_version}")
print(f"Finding count: {len(issues)}")

# Filter findings
processed = issues
processed = list(filter(lambda x: x['projectName'] == repository_name, processed)) # Ignore findings from other repositories in case it was imported incorrectly
processed = list(filter(lambda x: x['organization'] == organization_name, processed)) # Ignore findings from other organizations in case it was imported incorrectly
processed = list(filter(lambda x: x['severity'] in severity_mapping.keys(), processed)) # Filter out unknown/low severity findings
processed = list(filter(lambda x: x['status'] != 'CLOSED', processed)) # Ignore closed findings

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
    name = p['component'].split(':')[-1]
    test_id = file_to_id(name)
    results[ test_id ].append(p) # castle-benchmark_Tests-C:CASTLE-22-1.c -> 22-1

# Create report
test_list = []
for i,t in enumerate(results):
    findings = []
    for finding in results[t]:
        findings.append({
            "severity": severity_mapping[ finding['severity'] ],
            "line": finding['line'],
            "cwe": '',
            "message": finding['message'],
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
