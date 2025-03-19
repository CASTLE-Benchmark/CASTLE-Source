import os
import json
import datetime


tool_name = 'codeql'
tool_version = '2.20.1'
report_file = 'tests/result-security-extended.sarif'
dataset_file = '../../dataset.json'
final_report = f'../../reports/{tool_name}-report.json'


# Load results
with open(report_file) as f:
    result_file = json.load(f)
    
with open(dataset_file, "r") as f:
    dataset = json.load(f)
    
results = result_file['runs'][0]['results']
ds_tests = dataset['tests']


# Create a dictionary of tests with their grouped findings
tests = {}
for t in ds_tests:
    tests[t['id']] = []
    
def file_to_id(file: str) -> str:
    file = file.split('/')[-1] # Remove path
    file = file.split('.')[0] # Remove extension
    return file.removeprefix('CASTLE-')

for result in results:
    test_name = result['locations'][0]['physicalLocation']['artifactLocation']['uri'].split('/')[-1]
    test_id = file_to_id(test_name)
    tests[test_id].append(result)


# Create report
rules = result_file['runs'][0]['tool']['driver']['rules']
test_list = []
for i,t in enumerate(tests):
    ID = i+1
    
    findings = []
    for f in tests[t]:
        severity = "",
        cwe = 0
        # Search rule description
        for r in rules:
            if r['id'] == f['ruleId']:
                severity = r['properties']['problem.severity']
                cwe_list = list(filter(lambda x: x.startswith('external/cwe/'), r['properties']['tags']))
                cwe = int(cwe_list[0].removeprefix('external/cwe/cwe-'))
                break
        
        findings.append({
            "severity": severity,
            "line": f['locations'][0]['physicalLocation']['region']['startLine'],
            "cwe": cwe,
            "message": f['message']['text'],
        })
    
    test_list.append({
        "id": t,
        "findings": findings,
        "report": json.dumps(tests[t])
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

# Examples

'''
                    "rules": [
                        {
                            "id": "cpp/unsafe-strncat",
                            "name": "cpp/unsafe-strncat",
                            "shortDescription": {
                                "text": "Potentially unsafe call to strncat"
                            },
                            "fullDescription": {
                                "text": "Calling 'strncat' with an incorrect size argument may result in a buffer overflow."
                            },
                            "defaultConfiguration": {
                                "enabled": true,
                                "level": "warning"
                            },
                            "properties": {
                                "tags": [
                                    "reliability",
                                    "correctness",
                                    "security",
                                    "external/cwe/cwe-788",
                                    "external/cwe/cwe-676",
                                    "external/cwe/cwe-119",
                                    "external/cwe/cwe-251"
                                ],
                                "description": "Calling 'strncat' with an incorrect size argument may result in a buffer overflow.",
                                "id": "cpp/unsafe-strncat",
                                "kind": "problem",
                                "name": "Potentially unsafe call to strncat",
                                "precision": "high",
                                "problem.severity": "warning",
                                "security-severity": "9.3"
                            }
                        },
...
'''

