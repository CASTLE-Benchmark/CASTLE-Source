import os
import re
import hashlib
import json
import sys
import datetime

import yaml
import lizard
import tiktoken

from complexity import maintainability_index, halstead_volume


dataset_name = 'CASTLE-Benchmark'
dataset_version = '1.2'
dataset_authors = [
    'Richard A. Dubniczky',
    'Krisztofer Zoltan Horvat',
    'Tamas Bisztray',
    'Mohamed Amine Ferrag',
    'Lucas C. Cordeiro',
    'Norbert Tihanyi'
]
csv_target = 'tests.csv'
json_target = 'dataset.json'
indent_dataset_json = 4
dataset_directory_path = './dataset'
test_prefix = 'CASTLE-'
test_postfix = '.c'
re_line_marker = r'\s*\/\/\s*\{!LINE\}'
re_line_equals = r'^\s*===.*$'
re_multiline_comment = r'/\*(.*?)\*/'
cwe_collection_path = 'cwe-collection.yaml'
llm_tokenizer_encoding = 'cl100k_base'
llm_tokenizer_parameter = 'cl100k_base_tokens'
llm_prompt_file = 'prompt.txt'
expected_test_per_cwe = 10
expected_vulnerable_per_cwe = 6
parse_single_file = False
single_test_id = None
file_paths = []
files = []
IDs = []
tests = []
cwe_collection = None
llm_tokenizer = tiktoken.get_encoding(llm_tokenizer_encoding)


# The checking mode: single or all
if len(sys.argv) > 1:
    print(f'Parsing single test: {sys.argv[1]}')
    parse_single_file = True
    single_test_id = sys.argv[1]
else:
    print('Parsing all tests')


# Get sorted test files
print(f'Dataset directory: {os.path.join(os.getcwd(), dataset_directory_path)}')
for root, _, f in os.walk(dataset_directory_path):
    for file in f:
        assert file.startswith(test_prefix) and file.endswith(test_postfix), f"Invalid file name: {root}/{file}"
        file_paths.append(os.path.join(root, file))
def test_sort_value(path):
    filename = path.split('/')[-1]
    id = filename[len(test_prefix):-len(test_postfix)]
    id_parts = id.split('-')
    cwe = int(id_parts[0])
    num = int(id_parts[1])
    return cwe * 1000000 + num # Sort by CWE and then by number
file_paths.sort(key=test_sort_value)
print(f'Found {len(file_paths)} files.')


# Extract test IDs
IDs = [ f.split('/')[-1][len(test_prefix):-len(test_postfix)] for f in file_paths ] # Convert filenames to test ID numbers
print(f'Extracted {len(IDs)} test IDs successfully')
assert len(IDs) == len(list(set(IDs))), "There are duplicates in the test IDs"
print(f'[PASS] Duplicate check')


# Load test files
print(f'Loading files...')
for i,path in enumerate(file_paths):
    with open(path, 'r') as f:
        files.append(f.read())
print(f'[PASS] All loaded successfully')


# Filter files in case of a single test
if parse_single_file:
    num_id = IDs.index(single_test_id)
    files = [ files[ num_id ] ]
    print(f'Parsing single file: {num_id}')


# Parse test files
print(f'Parsing files...')
for i,f in enumerate(files):
    ID = IDs[i]
    
    # Separate header and body
    header = re.match(re_multiline_comment, f, re.DOTALL)[1]
    body = re.sub(re_multiline_comment, '', f, flags=re.DOTALL)

    # Parse header
    header = re.sub(re_line_equals, '', header, flags=re.MULTILINE) # remove lines (===)
    parsed = yaml.safe_load(header)
    assert parsed is not None, f"Header parsing failed for {ID}"

    # Parse body
    lines = body.splitlines(False)
    while len(lines) > 0 and lines[0].strip() == '': # Remove starting empty lines
        lines.pop(0)
    assert len(lines) > 0, f"Empty test body for {ID}"

    # Find line markers (if any)
    parsed['lines'] = []
    for j, line in enumerate(lines):
        if re.search(re_line_marker, line):
            parsed['lines'].append(j+1)
            lines[j] = re.sub(re_line_marker, '', lines[j]) # remove line marker

    # Convert code back to string
    code = '\n'.join(lines)
    
    # Save metadata
    parsed['id'] = ID
    parsed['number'] = i+1
    parsed['hash'] = hashlib.md5(body.encode('utf-8')).hexdigest()
    parsed['line_count'] = len(lines)
    del parsed['author'] # We are not showing individual authors
    
    # Calculate NLoC & Cyclomatic Complexity
    parsed['cyclomatic_complexity'] = 0
    cc_res = lizard.analyze_file.analyze_source_code(parsed['name'], code)
    for cc in cc_res.function_list:
        parsed['cyclomatic_complexity'] += cc.cyclomatic_complexity
    parsed['nloc'] = cc_res.nloc # Non-comment and non-empty lines of code
    parsed['functions'] = len(cc_res.function_list)
    
    # Calculate Halstead Volume and Maintainability Index
    parsed['halstead_volume'] = halstead_volume(code)
    parsed['maintainability_index'] = maintainability_index(parsed['cyclomatic_complexity'], parsed['nloc'], parsed['halstead_volume'])
    
    assert parsed['nloc'] > 0, f"Invalid NLoC: {parsed['nloc']} for {ID}"
    assert parsed['functions'] > 0, f"Invalid function count: {parsed['functions']} for {ID}"
    assert parsed['cyclomatic_complexity'] > 0, f"Invalid cyclomatic complexity: {parsed['cyclomatic_complexity']} for {ID}"
    
    # Calculate LLM token size
    parsed[llm_tokenizer_parameter] = len(llm_tokenizer.encode(code))
    
    # Save code
    parsed['code'] = code
    
    tests.append(parsed)
    print(f'   [{i+1}/{len(files)}] {parsed["id"]} cwe={parsed["cwe"]} vuln_lines={len(parsed["lines"])} lines={parsed["line_count"]} nloc={parsed["nloc"]} functions={parsed["functions"]} cc={parsed["cyclomatic_complexity"]} tokens={parsed[llm_tokenizer_parameter]} hv={parsed["halstead_volume"]} mi={parsed["maintainability_index"]}')
print(f'All tests parsed successfully')


# Output parsed data in single file mode
if parse_single_file:
    code = tests[0]['code']
    tests[0]['code'] = '...'
    target_lines = tests[0]['lines']
    code = '\n'.join([f'{i+1:3} {"@" if i+1 in target_lines  else "|"} {line}' for i, line in enumerate(code.splitlines())])
    
    print(json.dumps(tests[0], indent=4))
    print('\n')
    print(code)
    sys.exit(0)


# Run in-depth checks to filter out simple mistakes when creating the tests
for i, t in enumerate(tests):
    assert t['name'] == f'{test_prefix}{t["id"]}{test_postfix}', f"Name-ID mismatch {t['name']} != {test_prefix}{t['id']}{test_postfix}"
    assert len(t['lines']) != 0 if t['vulnerable'] else True, f"Line number has to be marked if flagged as vulnerable {t['name']}, {t['lines']}"
    assert len(t['lines']) == 0 if not t['vulnerable'] else True, f"Line marked as vulnerable in non-vulnerable test {t['name']}, {t['lines']}"
    assert t['cwe'] > 0, f"CWE have to be a valid number. Invalid CWE number: {t['cwe']}"
    assert t['line_count'] > 1, f"Test has to be more than 1 lines. Possible invalid test. {t['name']}"
    assert t['vulnerable'] if int(t['id'].split('-')[1]) <= expected_vulnerable_per_cwe else True, f"The first {expected_vulnerable_per_cwe} have to be vulnerable. Exception: {t['id']}"
    assert not t['vulnerable'] if int(t['id'].split('-')[1]) > expected_vulnerable_per_cwe else True, f"The last {expected_test_per_cwe-expected_vulnerable_per_cwe} have to be not vulnerable. Exception: {t['id']}"
    assert t['compile'].count(f'{test_prefix}{t["id"]}'), f"Compile command has to contain the test name twice, both for the input and the output files: {t['id']} -> `{t['compile']}`"
print(f'[PASS] Content checks')


# Calculate global statistics
statistics = {
    'characters': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'line_count': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'nloc': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    llm_tokenizer_parameter: {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'functions': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'cyclomatic_complexity': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'halstead_volume': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    },
    'maintainability_index': {
        'total': 0,
        'average': None,
        'min': None,
        'max': None
    }
}
for t in tests:
    # Characters
    statistics['characters']['total'] += len(t['code'])
    if statistics['characters']['min'] is None or len(t['code']) < statistics['characters']['min']:
        statistics['characters']['min'] = len(t['code'])
    if statistics['characters']['max'] is None or len(t['code']) > statistics['characters']['max']:
        statistics['characters']['max'] = len(t['code'])
    
    # Line Count
    statistics['line_count']['total'] += t['line_count']
    if statistics['line_count']['min'] is None or t['line_count'] < statistics['line_count']['min']:
        statistics['line_count']['min'] = t['line_count']
    if statistics['line_count']['max'] is None or t['line_count'] > statistics['line_count']['max']:
        statistics['line_count']['max'] = t['line_count']
    
    # NLoC
    statistics['nloc']['total'] += t['nloc']
    if statistics['nloc']['min'] is None or t['nloc'] < statistics['nloc']['min']:
        statistics['nloc']['min'] = t['nloc']
    if statistics['nloc']['max'] is None or t['nloc'] > statistics['nloc']['max']:
        statistics['nloc']['max'] = t['nloc']
    
    # LLM Tokens
    statistics[llm_tokenizer_parameter]['total'] += t[llm_tokenizer_parameter]
    if statistics[llm_tokenizer_parameter]['min'] is None or t[llm_tokenizer_parameter] < statistics[llm_tokenizer_parameter]['min']:
        statistics[llm_tokenizer_parameter]['min'] = t[llm_tokenizer_parameter]
    if statistics[llm_tokenizer_parameter]['max'] is None or t[llm_tokenizer_parameter] > statistics[llm_tokenizer_parameter]['max']:
        statistics[llm_tokenizer_parameter]['max'] = t[llm_tokenizer_parameter]
    
    # Functions
    statistics['functions']['total'] += t['functions']
    if statistics['functions']['min'] is None or t['functions'] < statistics['functions']['min']:
        statistics['functions']['min'] = t['functions']
    if statistics['functions']['max'] is None or t['functions'] > statistics['functions']['max']:
        statistics['functions']['max'] = t['functions']
        
    # Cyclomatic Complexity
    statistics['cyclomatic_complexity']['total'] += t['cyclomatic_complexity']
    if statistics['cyclomatic_complexity']['min'] is None or t['cyclomatic_complexity'] < statistics['cyclomatic_complexity']['min']:
        statistics['cyclomatic_complexity']['min'] = t['cyclomatic_complexity']
    if statistics['cyclomatic_complexity']['max'] is None or t['cyclomatic_complexity'] > statistics['cyclomatic_complexity']['max']:
        statistics['cyclomatic_complexity']['max'] = t['cyclomatic_complexity']
        
    # Halstead Volume
    statistics['halstead_volume']['total'] += t['halstead_volume']
    if statistics['halstead_volume']['min'] is None or t['halstead_volume'] < statistics['halstead_volume']['min']:
        statistics['halstead_volume']['min'] = t['halstead_volume']
    if statistics['halstead_volume']['max'] is None or t['halstead_volume'] > statistics['halstead_volume']['max']:
        statistics['halstead_volume']['max'] = t['halstead_volume']
        
    # Maintainability Index
    statistics['maintainability_index']['total'] += t['maintainability_index']
    if statistics['maintainability_index']['min'] is None or t['maintainability_index'] < statistics['maintainability_index']['min']:
        statistics['maintainability_index']['min'] = t['maintainability_index']
    if statistics['maintainability_index']['max'] is None or t['maintainability_index'] > statistics['maintainability_index']['max']:
        statistics['maintainability_index']['max'] = t['maintainability_index']

statistics['characters']['average'] = statistics['characters']['total'] / len(tests)
statistics['line_count']['average'] = statistics['line_count']['total'] / len(tests)
statistics['nloc']['average'] = statistics['nloc']['total'] / len(tests)
statistics[llm_tokenizer_parameter]['average'] = statistics[llm_tokenizer_parameter]['total'] / len(tests)
statistics['functions']['average'] = statistics['functions']['total'] / len(tests)
statistics['cyclomatic_complexity']['average'] = statistics['cyclomatic_complexity']['total'] / len(tests)
statistics['halstead_volume']['average'] = statistics['halstead_volume']['total'] / len(tests)
statistics['maintainability_index']['average'] = statistics['maintainability_index']['total'] / len(tests)


# Load CWE Collection
with open(cwe_collection_path, 'r') as f:
    cwe_collection = yaml.safe_load(f)
print(f'Loaded CWE collection: {cwe_collection_path}')


# Validate that test CWEs are all in the collection
cwe_collection_list = [ int(cwe) for cwe in list(cwe_collection.keys()) ]
for t in tests:
    assert t['cwe'] in cwe_collection_list if t['cwe'] != 0 else True, f"CWE {t['cwe']} of test {t['name']} not found in collection"
print(f'[PASS] CWE collection check')


# Calculate total LLM token requirements
with open(llm_prompt_file, 'r') as f:
    llm_prompt = f.read()
llm_prompt_tokens = len(llm_tokenizer.encode(llm_prompt))
total_llm_tokens = statistics[llm_tokenizer_parameter]['total'] + llm_prompt_tokens * len(tests) # Add prompt tokens to each test


# Generate JSON
dataset = {
    'dataset': dataset_name,
    'version': dataset_version,
    'date': datetime.datetime.now().isoformat(),
    'test_count': len(tests),
    'vulnerable_count': len([ t for t in tests if t['vulnerable'] ]),
    'non_vulnerable_count': len([ t for t in tests if not t['vulnerable'] ]),
    'total_llm_input_tokens': total_llm_tokens,
    'authors': dataset_authors,
    'statistics': statistics,
    'tests': tests,
    'cwes': cwe_collection,
    'prompt': llm_prompt
}
with open(json_target, 'w') as f:
    if indent_dataset_json:
        json.dump(dataset, f, indent=indent_dataset_json)
    else:
        json.dump(dataset, f)
print(f'Saved JSON: {json_target}')


# Generate stats
print('===== STATS =====')
print(f'Parsed {len(tests)} tests')
print(f'Vulnerable: {dataset["vulnerable_count"]} ({dataset["vulnerable_count"]/len(tests)*100:.1f}%)')
print(f'Non-vulnerable: {len(tests) - dataset["vulnerable_count"]} ({(len(tests) - dataset["vulnerable_count"])/len(tests)*100:.1f}%)')
stat_cwe_list = list(set([ str(t['cwe']) for t in tests ]))
print(f'Unique CWEs: {len(stat_cwe_list)}')
print(f'CWEs: {", ".join(stat_cwe_list)}')
print(f'LLM prompt tokens: {llm_prompt_tokens}')
print(f'Total LLM Input tokens: {total_llm_tokens}')
print(f'Code stats:')
print(f"+ Characters: {statistics['characters']['total']:.1f} (avg:{statistics['characters']['average']:.1f}, min: {statistics['characters']['min']}, max: {statistics['characters']['max']})")
print(f"+ Lines of code: {statistics['line_count']['total']:.1f} (avg:{statistics['line_count']['average']:.1f}, min: {statistics['line_count']['min']}, max: {statistics['line_count']['max']})")
print(f"+ NLoC: {statistics['nloc']['total']:.1f} (avg:{statistics['nloc']['average']:.1f}, min: {statistics['nloc']['min']}, max: {statistics['nloc']['max']})")
print(f"+ LLM Tokens: {statistics[llm_tokenizer_parameter]['total']:.1f} (avg:{statistics[llm_tokenizer_parameter]['average']:.1f}, min: {statistics[llm_tokenizer_parameter]['min']}, max: {statistics[llm_tokenizer_parameter]['max']})")
print(f"+ Functions: {statistics['functions']['total']:.1f} (avg:{statistics['functions']['average']:.1f}, min: {statistics['functions']['min']}, max: {statistics['functions']['max']})")
print(f"+ Cyclomatic Complexity: {statistics['cyclomatic_complexity']['total']:.1f} (avg:{statistics['cyclomatic_complexity']['average']:.1f}, min: {statistics['cyclomatic_complexity']['min']}, max: {statistics['cyclomatic_complexity']['max']})")
print(f"+ Halstead Volume: {statistics['halstead_volume']['total']:.1f} (avg:{statistics['halstead_volume']['average']:.1f}, min: {statistics['halstead_volume']['min']:.1f}, max: {statistics['halstead_volume']['max']:.1f})")
print(f"+ Maintainability Index: {statistics['maintainability_index']['total']:.1f} (avg:{statistics['maintainability_index']['average']:.1f}, min: {statistics['maintainability_index']['min']:.1f}, max: {statistics['maintainability_index']['max']:.1f})')")
