import os
import csv
import json
import datetime

tool_name = 'coverity'
tool_version = '2024.12.0'
report_folder = '../../reports'
report_suffix = '-report.json'
report_file = 'coverity-report.csv'
dataset_file = '../../dataset.json'
# Filters
project_name = 'DCI_CA_Testing'
accepted_statuses = [ 'opened' ]
accepted_domains = [ 'Static Analysis (SAST)']

# Load report
# Issue name,Severity,Path,Domain,Project,Branch,Status,Deep link,Related to
findings = []
with open(report_file, 'r') as f:
    reader = csv.reader(f)
    for row in reader:
        findings.append({
            # The values are stripped because there are some weird spaces before and after the values for some reason
            'issue': row[0].strip(),
            'severity': row[1].strip(),
            'path': row[2].strip(),
            'domain': row[3].strip(),
            'project': row[4].strip(),
            'branch': row[5].strip(),
            'status': row[6].strip(),
            'deep_link': row[7].strip(),
            'related_to': row[8].strip(),
            '_original': ','.join([f'"{r}"' for r in row])
        })
findings = findings[1:] # remove headers
print(f'Loaded {len(findings)} findings')

# Load dataset
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']


# Filter findings
filtered = findings
filtered = filter(lambda f: f['project'] == project_name, filtered)
filtered = filter(lambda f: f['status'] in accepted_statuses, filtered)
filtered = filter(lambda f: f['domain'] in accepted_domains, filtered)
filtered = list(filtered)
print(f'Filtered to {len(filtered)} findings')


def parse_finding(f: dict[str,str]):
    cwes = f['related_to'].split(', ')
    cwes = [cwe for cwe in cwes if cwe.startswith('CWE-')]
    cwe = None
    if len(cwes) == 0:
        cwe = 0
    else:
        cwe = int(cwes[0].split('-')[1])
    return {
        'severity': f['severity'],
        'line': 0, # The CSV does not return the line number...
        'cwe': cwe,
        'message': f['issue'],
    }


findings = {}
reports = {}
for t in tests:
    findings[t['name']] = []
    reports[t['name']] = []
    
for f in filtered:
    name = f['path']
    assert name in findings and name in reports
    reports[name].append(f['_original'])
    findings[name].append(parse_finding(f))

test_list = []
for t in tests:
    ID = t['id']
    test_list.append({
        'id': ID,
        'findings': findings[t['name']],
        'report': '\n'.join(reports[t['name']])
    })
    

# Export report
final_report = os.path.join(report_folder, f'{tool_name}{report_suffix}')
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