# After using the create_manual_prompt.py script and running the manual tests
# the results are stored back in the propts file. This script extract the results to a JSON report
# Usage: python scripts/process_manual_prompt.py

import json
import datetime
import re
import sys


dataset_file = 'dataset.json'
prompt_file = 'prompt.txt'
report_file = 'report.md'
tool_name = 'o1'
final_report = f'reports/{tool_name}-report.json'


# Read data
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']
    
with open(prompt_file, 'r') as f:
    prompt = f.read()
    
with open(report_file, 'r') as f:
    report = f.read()

# Write tests
# with open(output_file, 'w') as f:
#     for t in tests:
#         code_snippet = t['code']
#         markdown_code = f'```c\n{code_snippet}\n```'
#         total_size += len(llm_tokenizer.encode(markdown_code))
        
#         f.write(f'# ========================== {t["id"]} ==========================\n\n{prompt}{code_snippet}\n\n')

def get_between(text, start_substring, end_substring):
    start_index = text.find(start_substring)
    if start_index == -1:
        return None
    start_index += len(start_substring)
    end_index = text.find(end_substring, start_index)
    if end_index == -1:
        return None
    return text[start_index:end_index]

def parse_report(test, report: str):
    # 2 cases: empty or not empty
    
    report_lines = report.split('\n')
    
    # Search for start line
    findings_start = None
    for i in range(len(report_lines)-1, 0, -1):
        if report_lines[i].startswith('['):
            if report_lines[i].startswith('[]'):
                return []
            findings_start = i
            break
    if findings_start is None:
        return []
        
    # Search for end line
    findings_end = None
    for i in range(findings_start, len(report_lines)):
        if report_lines[i].startswith(']'):
            findings_end = i
            break
    if findings_end is None:
        return []
        
    findings_lines = report_lines[findings_start:findings_end+1]
    findings_text = '\n'.join(findings_lines)

    # Parse JSON
    try:
        findings = json.loads(findings_text)
    except json.JSONDecodeError as e:
        return []
    
    # Invalid JSON list
    if not isinstance(findings, list):
        return []
    
    # Correct for line number errors
    # search for line number with content
    cleaned_findings = []
    lines = test['code'].split('\n')
    for f in findings:
        # Validate keys
        validate_keys = ['severity', 'line', 'cwe', 'message', 'line_content']
        if not all(k in f for k in validate_keys):
            continue
        
        # Validate types
        if (not isinstance(f['severity'], str) or
            not isinstance(f['line'], int) or
            not isinstance(f['cwe'], int) or
            not isinstance(f['message'], str) or
            not isinstance(f['line_content'], str)):
            continue
        
        # Lowercase severity
        f['severity'] = f['severity'].lower()
        
        line_content = f['line_content']
        del f['line_content']
        
        # Clamp line number
        if f['line'] < 1:
            f['line'] = 1
        elif f['line'] > len(lines):
            f['line'] = len(lines)
        
        # Line number is correct
        if lines[ f['line']-1 ].strip() == line_content.strip():
            cleaned_findings.append(f)
            continue
        
        # Search for line number
        matches = []
        for i, l in enumerate(lines):
            if l.strip() == line_content.strip():
                matches.append(i)
                break
        
        # Select closest
        if len(matches) > 0:
            closest_index = min(matches, key=lambda x: abs(x - f['line'] - 1))
            f['line'] = closest_index + 1
            
        cleaned_findings.append(f)
            
    return findings

results = []
finding_count = 0
for i,t in enumerate(tests):
    # The last one is added afterwards
    if i == len(tests) - 1:
        continue
    
    # Extract text
    test_text = get_between(
        report,
        f'# ========================== {tests[i]["id"]} ==========================',
        f'# ========================== {tests[i+1]["id"]} =========================='
    )
    assert test_text is not None, f'{t["id"]}'
    
    # Remove the prompt & code snippet
    code = t['code']
    question_lines = len(prompt.split('\n')) + len(code.split('\n')) + 1
    extracted_text = '\n'.join(test_text.split('\n')[question_lines:])
    
    # Parse findings
    findings = parse_report(t, extracted_text)
    finding_count += len(findings)
    
    results.append({
        'id': t['id'],
        'findings': findings if findings else [],
        'report': extracted_text
    })
    
# Last test
last_text = tests[-1]
test_text = report.index(f'# ========================== {last_text["id"]} ==========================')
last_extracted_text = report[test_text:]
last_question_lines = len(prompt.split('\n')) + len(code.split('\n')) + 1

 # Parse findings
last_findings = parse_report(t, extracted_text)
finding_count += len(findings)
    
results.append({
    'id': last_text['id'],
    'findings': last_findings if last_findings else [],
    'report': last_extracted_text
})


print(f'Findings: {finding_count}')

# Export report
with open(final_report, 'w') as f:
    f.write(json.dumps({
        "dataset": dataset['dataset'],
        "version": dataset['version'],
        "tool": tool_name,
        "date": datetime.datetime.now().isoformat(),
        "runtime": 0,
        "tool_type": "llm",
        "tests": results
    }, indent=4))
print(f'Report saved: {final_report}')