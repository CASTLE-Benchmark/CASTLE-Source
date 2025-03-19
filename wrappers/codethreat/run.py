import json
import datetime


tool_name = 'codethreat'
report_file = 'codethreat.json'
repository_name = 'Tests-C'
dataset_file = '../../dataset.json'
tool_version = ''
final_report = f'../../reports/{tool_name}-report.json'


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
processed = list(filter(lambda x: x['issue_state']['project_name'] == repository_name, processed)) # Ignore findings from other repositories in case it was imported incorrectly
processed = list(filter(lambda x: x['issue_state']['relations']['scanBranches'][0]['branch_name'] == 'main', processed)) # Make sure there are no duplicated findings because of multiple branches

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
    results[ file_to_id( p['flow_steps'][0]['document_name'].split('/')[-1] ) ].append(p)
    

# Create report
test_list = []
for i,t in enumerate(results):
    findings = []
    for finding in results[t]:
        findings.append({
            "severity": finding['issue_state']['severity'],
            "line": finding['flow_steps'][0]['start_linenumber'],
            "cwe": '',
            "message": finding['kb_fields']['summary']['en'],
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
