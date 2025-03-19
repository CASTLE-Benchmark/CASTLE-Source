import re
import json
import datetime

tool_name = 'gitlab-sast'
report_path = "gl-sast-report.json"
dataset_file = '../../dataset.json'
final_report = f'../../reports/{tool_name}-report.json'

def vulnerability_to_finding (vuln: dict) -> dict | None:
    file_pattern = r".*\-(\d+-\d+)\.c"
    match = re.match(file_pattern, vuln['location']['file'])
    if not match:
        return None
    test_index = match.group(1)
    severity = vuln['severity'].lower()
    line = vuln['location']['start_line']
    cwe_descriptions = [desc for desc in vuln['identifiers'] if desc['type'] == 'cwe']
    cwe = int(cwe_descriptions[0]['value']) if len(cwe_descriptions) > 0 else 0
    message = f'{vuln["name"]} - {vuln["description"]}'
    return {
        'test_ind': test_index,
        'severity': severity,
        'line': line,
        'cwe': cwe,
        'message': message
    }

def format_finding(finding: dict) -> dict:
    return {
        'severity': finding['severity'],
        'line': int(finding['line']),
        'cwe': finding['cwe'],
        'message': finding['message']
    }

# Load report
report = None
with open(report_path, "r") as f:
    report = json.load(f)

# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

# Process report
findings = []
for vuln in report['vulnerabilities']:
    finding = vulnerability_to_finding(vuln)
    if finding is not None:
        findings.append(finding)

# Create reports
reports = []
for test in tests:
    findings_for_test = [format_finding(f) for f in findings if f['test_ind'] == test['id']]
    reports.append({
        "id": test['id'],
        "findings": findings_for_test,
        "report": "\n\n".join([f['message'] for f in findings_for_test])
    })

# Export report
with open(final_report, 'w') as f:
    f.write(json.dumps({
        "dataset": dataset['dataset'],
        "version": dataset['version'],
        "tool": tool_name,
        "date": datetime.datetime.now().isoformat(),
        "runtime": 0,
        "tests": reports
    }, indent=4))
print(f'Report saved: {final_report}')