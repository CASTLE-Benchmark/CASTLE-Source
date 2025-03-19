# Prune a report to limit the size of the report by removing the beginning of the content of each test outputs.
# This does not affect the parsed report

import os
import sys
import json


report_directory = 'reports'
report_suffix = '-report.json'
new_report_suffix = '-report.min.json'
max_report_length = 200_000 # Character limit
prune_point_marker = '=== CUT DUE TO SIZE LIMIT ==='

report_name = sys.argv[1]
if len(sys.argv) < 2:
    print("Usage: python scripts/prune_report.py <report>")
    sys.exit(0)
report_path = os.path.join(report_directory, report_name + report_suffix)
    
if not os.path.exists(report_path):
    print(f"Report does not exist: {report_path}")
    sys.exit(1)
    
with open(report_path, 'r') as f:
    report = json.load(f)
tests = report['tests']

for t in tests:
    if len(t['report']) > max_report_length:
        l = len(t['report'])
        t['report'] = prune_point_marker + t['report'][-max_report_length:]
        print(f"[{t['id']}] {l//1024}kb -> {len(t['report'])//1024}kb", flush=True)
    else:
        print(f"[{t['id']}] ✓", flush=True)

with open(os.path.join(report_directory, report_name + new_report_suffix), 'w') as f:
    json.dump(report, f, indent=4)
    