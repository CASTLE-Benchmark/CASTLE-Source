# Evaluates the results that were generated using the prepare_human_test.py script. The humans fill their guesses for each test and this script creates the report

import json
import re

path = 'human_result/test1/'
mappings_file = 'mappings.json'
dataset_file = '../../dataset.json'
error_line_text = '// Line of error (or 0 if there is none): '

with open(path + mappings_file, 'r') as f:
    mappings = json.load(f)
    
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
    tests = dataset['tests']
    
results = {}
for filename in mappings:
    with open(path + filename, 'r') as f:
        code = f.read()
        error_line = code.find(error_line_text)
        if error_line == -1:
            error_line = 0
        else:
            error_line = re.findall(r'\/\/ Line of error \(or 0 if there is none\): (.+)', code, flags=re.DOTALL)[0]
                
        original_test = mappings[filename]
        correct_lines = list(filter(lambda test: test['name'] == original_test, dataset['tests']))[0]['lines']
        selected = re.sub(r'[^0-9]', '', error_line)
        if selected == '':
            continue
        selected = int(selected)
        
        # Evaluate
        tp = selected != 0 and selected in correct_lines
        fp = selected != 0 and selected not in correct_lines
        fn = selected == 0 and correct_lines != []
        tn = selected == 0 and correct_lines == []
        
        results[original_test] = {
            'selected': selected,
            'correct': ' '.join(map(str, correct_lines)),
            'tp': 1 if tp else 0,
            'fp': 1 if fp else 0,
            'fn': 1 if fn else 0,
            'tn': 1 if tn else 0
        }
        
        #print(f'Test name: {mappings[filename]}')
with open('human_results.csv', 'w') as f:
    f.write('Test name,Selected line,Correct line,TP,TN,FP,FN\n')
    for i in results:
        item = results[i]
        f.write(f'{i},{results[i]["selected"]},{results[i]["correct"]},{item['tp']},{item['fp']},{item['fn']},{item['tn']}\n')
        