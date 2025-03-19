# Upgrade a report from 1.1 format to 1.2
# Used in cases when a lengthy report has been generated and it's modified instead of re-run.

import json


with open('report.json', 'r') as f:
    report = json.load(f)
    
for t in report['tests']:
    test_id = t['name'].removeprefix('CASTLE-').removesuffix('.c')
    t['id'] = test_id
    del t['name']
    
with open('report2.json', 'w') as f:
    json.dump(report, f, indent=4)
