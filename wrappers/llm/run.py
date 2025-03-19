import os
import re
import sys
import json
import timeit
import datetime

from openai import OpenAI

# Burnt in runner for now
dataset_file = '../../dataset.json'
llm_provider_url = 'https://api.deepseek.com' # https://openrouter.ai/api/v1
llm_model = 'deepseek-reasoner'
tool_name = llm_model
final_report = f'../../reports/{tool_name}-report.json'
secrets_file = 'secrets.json'
system_prompt = None
system_prompt_file = '../../prompt.txt'
max_tokens = 4000
reports = []

# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

# Load Secrets
with open(secrets_file, 'r') as f:
    secrets = json.loads(f.read())
client = OpenAI(
    base_url = llm_provider_url,
    api_key = secrets['api_key']
)

# Load Prompt
with open(system_prompt_file, 'r') as f:
    system_prompt = f.read()
    
# Continue feature
start_index = 0
if os.path.exists(final_report):
    with open(final_report, 'r') as f:
        report = json.load(f)
        report_length = len(report['tests'])
        if report['tool'] == tool_name and report_length < len(tests):
            print(f'Continuing report from {report_length}/{len(tests)}')
            start_index = report_length - 1
            reports = report['tests']
            tests = tests[report_length:]

print(f"Dataset: {dataset['dataset']}")
print(f"Version: {dataset['version']}")
print(f"Tests: ", len(tests))

def parse_report(test, report: str):
    report_json_regex = r'```json(?P<json>[\s\S]*)```'
    match = re.search(report_json_regex, report, flags=re.MULTILINE | re.DOTALL)
    if not match:
        return []

    # Parse JSON
    try:
        findings = json.loads(match.group('json'))
    except:
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

# Check run mode
single_test = False
if len(sys.argv) > 1:
    single_test = True
    test_id = sys.argv[1]
    tests = [t for t in tests if t['id'] == test_id]

# Run tests
start_time = timeit.default_timer()
try:
    for i, t in enumerate(tests, start=start_index):
        ID = i+1
        print(f'[{ID}/{len(tests)}] {t['id']} -> ', end='', flush=True)
        test_time = timeit.default_timer()
            
        code = t["code"]
        
        completion = client.chat.completions.create(
            model = llm_model,
            messages = [
                {'role': 'system', 'content': system_prompt},
                {'role': 'user', 'content': f'```{code}\n```'}
            ],
            max_tokens = max_tokens
        )
        
        # Gather report
        report = completion.choices[0].message.content
        findings = parse_report(t, report)
        if single_test:
            print(report)
            sys.exit(0)
        
        reports.append({
            "id": t["id"],
            "findings": findings,
            "report": report
        })

        print(f'#{len(findings)} {(timeit.default_timer()-test_time):.2f}s')
        
        # Export report
        with open(final_report, 'w') as f:
            f.write(json.dumps({
                "dataset": dataset['dataset'],
                "version": dataset['version'],
                "tool": tool_name,
                "date": datetime.datetime.now().isoformat(),
                "runtime": timeit.default_timer() - start_time,
                "tool_type": "llm",
                "tests": reports
            }, indent=4))
except Exception as e:
    print(f'Error Interrupted before completion! The current state is saved.')
    pass

stop_time = timeit.default_timer()
runtime = stop_time - start_time
print(f'Runtime: {runtime:.2f}s')
print(f'Runtime avg per test: {(runtime/len(tests)):.2f}s')
