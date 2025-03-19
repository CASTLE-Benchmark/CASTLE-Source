import os
import re
import json
import datetime

# For processing
tool_name = 'jit'
dataset_file = '../../dataset.json'
final_report = f'../../reports/{tool_name}-report.json'
log_folder = 'logs'

def is_log_line(line: str) -> bool:
    log_pattern = r"(\d+)\-(\d+)\-(\d+) (\d+):(\d+):(\d+) | INFO | Control#gitleaks"
    match = re.search(log_pattern, line)
    return bool(match)

def process_log_file(log_file: str) -> list[dict]:
    with open(log_file, 'r') as f:
        loglines = f.readlines()
    findings_start_line_inds = [i for i, line in enumerate(loglines) if 'Unified findings:' in line]
    if len(findings_start_line_inds) != 1:
        return []
    findings_start_line_ind = findings_start_line_inds[0] + 1
    findings_end_line_ind = findings_start_line_ind
    while findings_end_line_ind < len(loglines) and not is_log_line(loglines[findings_end_line_ind]):
        findings_end_line_ind += 1
    
    findings_str = '\n'.join(loglines[findings_start_line_ind:findings_end_line_ind])
    findings = json.loads(findings_str)

    is_secret_detection = (len([line for line in loglines if 'Secret Detection' in line]) > 0)
    for finding in findings:
        finding['is_secret_detection'] = is_secret_detection
    return findings

def process_log_files(log_folder: str) -> list[dict]:
    findings = []
    for file in os.listdir(log_folder):
        file_path = os.path.join(log_folder, file)
        if os.path.isfile(file_path) and file.endswith('.log'):
            findings += process_log_file(file_path)
    return findings

def get_cwe(log: dict) -> int:
    if 'cwes' not in log or len(log['cwes']) == 0:
        return 0
    
    cwe_pattern = r"CWE-(\d+)"
    match = re.search(cwe_pattern, log['cwes'][0])
    return int(match.group(1)) if match else 0

def log_to_finding(log: dict) -> dict | None:
    file_pattern = r".*\-(\d+-\d+)\.c"
    match = re.match(file_pattern, log['filename'])
    if not match:
        return None
    test_index = match.group(1)
    line = int(log['line_range'].split('-')[0])
    cwe = 798 if log['is_secret_detection'] else get_cwe(log)
    message = log['issue_text']
    severity = log['issue_severity'].lower()
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

# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

process_log_files(log_folder)

# Logs to findings
logs = process_log_files(log_folder)
findings = []
for log in logs:
    finding = log_to_finding(log)
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