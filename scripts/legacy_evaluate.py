# legacy evaluation script for the old metric
# now prefer to use the charts.ipynb for the new metrics and charts

import os
import sys
import json

report_folder = 'reports'
report_postfix = '-report.json'
dataset_path = ''
report_name = ''
relevant_findings = [
    '', 'none', # systems that do not include severity we treat them as equal
    'warning', 'error', # systems using error type
    'medium', 'high', 'critical', # systems using severity type
]
points = {
    'tp': 5,
    'tn': 2,
    'fp': 1,
    'fn': 0
}
result_postfix = '-result'


# Load CLI parameters
if len(sys.argv) < 3:
    print(f'Usage: python {sys.argv[0]} <dataset_path> <report_name>')
    print(f'Example: python {sys.argv[0]} dataset.json cppcheck')
    sys.exit(1)

dataset_path = sys.argv[1]
report_name = sys.argv[2]
report_path = os.path.join(report_folder, report_name + report_postfix)
result_path = os.path.join(report_folder, report_name + result_postfix)


# Load files
print(f'Loading report from {report_path}...')
report = None
with open(report_path, 'r') as f:
    report = json.load(f)

print(f'Loading dataset from {dataset_path}...')
dataset = None
with open(dataset_path, 'r') as f:
    dataset = json.load(f)
    
    
# Parse CWEs
def cwe_collection_to_dict(cwe_collection: dict[str, any]) -> dict[int, list[int]]:
    cwe_dict = {
        0: []
    }
    for c in cwe_collection:
        cwes = [ int(c) ]
        cwes += [ int(list(p.keys())[0]) for p in cwe_collection[c]['children'] ]
        cwes += [ int(list(p.keys())[0]) for p in cwe_collection[c]['parents'] ]
        cwe_dict[int(c)] = cwes
    return cwe_dict
accepted_cwe_lists = cwe_collection_to_dict(dataset['cwes'])

# Parser functions
def filter_findings(parsed: list[dict]):
    filtered = [ f for f in parsed if f['severity'].lower() in relevant_findings ]
    return filtered


def is_cwe_match(collection, correct: int, reported: int):
    # Matches the correct
    if correct == reported:
        return True
    # Matches of of the children
    if reported in collection[str(correct)]['children']:
        return True
    # Matches one of the parents
    if reported in collection[str(correct)]['parents']:
        return True
    
    return False

def is_line_match(corrects: list[int], reported: int):
    return reported in corrects

def validate_findings(test: list[dict], findings: list[dict]):
    # True negative
    if len(findings) == 0 and not test['vulnerable']:
        return {
            "correct": True,
            "points": points['tn'],
            "actual": {
                "vulnerable": False,
                "line": 0,
                "cwe": 0,
                "message": '',
            },
            "expected": {
                "vulnerable": test['vulnerable'],
                "line": test['lines'],
                "cwe": [],
                "message": test['description'],
            },
            "false_positives": 0,
        }
        
    # False negative
    if len(findings) == 0 and test['vulnerable']:
        return {
            "correct": False,
            "points": points['fn'],
            "actual": {
                "vulnerable": False,
                "line": 0,
                "cwe": 0,
                "message": '',
            },
            "expected": {
                "vulnerable": test['vulnerable'],
                "line": test['lines'],
                "cwe": accepted_cwe_lists[test['cwe']],
                "message": test['description'],
            },
            "false_positives": 0,
        }
        
    # True positive
    matches = [ f for f in findings if f['line'] in test['lines'] or f['cwe'] in accepted_cwe_lists[test['cwe']] ]
    if len(matches) > 0:
        m = matches[0]
        false_positive_count = len(findings) - len(matches) # if two findings are reported on the same line, we count it as one, so we are not subtracting points. Sone systems may report on an issue in the given line multiple times, because they separate out the underlying cause and its effects. We are not deducting points for that.
        return {
            "correct": True,
            "points": points['tp'] - 0 if false_positive_count == 0 else points['fp'],
            "actual": {
                "vulnerable": True,
                "line": m['line'],
                "cwe": m['cwe'],
                "message": m['message'],
            },
            "expected": {
                "vulnerable": test['vulnerable'],
                "line": test['lines'],
                "cwe": accepted_cwe_lists[test['cwe']],
                "message": test['description'],
            },
            "false_positives": false_positive_count,
        }
        
    # False positive
    return {
        "correct": False,
        "points": points['fp'] * len(findings),
        "actual": {
            "vulnerable": True,
            "line": findings[0]['line'],
            "cwe": findings[0]['cwe'],
            "message": findings[0]['message'],
        },
        "expected": {
            "vulnerable": test['vulnerable'],
            "line": test['lines'],
            "cwe": [],
            "message": test['description'],
        },
        "false_positives": len(findings),
    }

print(f'Dataset: {report["dataset"]} v{report["version"]}')
print(f'Date: {report["date"]}')
print(f'Runtime: {report["runtime"]:.2f}s')

print(f'Processing {len(report["tests"])} results...')
results = []
for i, rep in enumerate(report['tests']):
    # Remove low level / irrelevant findings
    findings = filter_findings(rep['findings'])
    # Determine result
    result = validate_findings(dataset['tests'][i], findings)
    results.append(result)
    
print()
print('========== STATS ==========')
print()

vulnerable_count = len([ r for r in results if r['expected']['vulnerable'] ])
max_points = vulnerable_count * points['tp'] + (len(results) - vulnerable_count) * points['tn']
false_positive_count = sum([ r["false_positives"] for r in results ])
print(f'Total points: {sum([ r["points"] for r in results ])} / {max_points}')
print(f'Correct: {len([ r for r in results if r["correct"] ])}')
print(f'True positives: {len([ r for r in results if r["correct"] and r["actual"]["vulnerable"] ])}')
print(f'True negatives: {len([ r for r in results if r["correct"] and not r["actual"]["vulnerable"] ])}')
print(f'False positives: {false_positive_count}')
print(f'False negatives: {len([ r for r in results if not r["correct"] and not r["actual"]["vulnerable"] ])}')
print()

print('Writing results...')
with open(result_path + '.csv', 'w') as f:
    f.write("ID,Is Correct,Excpected Vulnerable,Actual Vulnerable,Expected CWEs,Actual CWE,Expected Line,Actual Line,Message,Points\n")
    for i, r in enumerate(results):
        f.write(','.join([
            str(i+1),
            str(r['correct']),
            str(r['expected']['vulnerable']),
            str(r['actual']['vulnerable']),
            ' '.join([ str(c) for c in r['expected']['cwe'] ]),
            str(r['actual']['cwe']),
            ' '.join([ str(c) for c in r['expected']['line'] ]),
            str(r['actual']['line']),
            '"' + r['actual']['message'] + '"',
            str(r['points'])
        ]) + '\n')