# Creates a directory with of a randomized set of the tests for humans to manually evaluate. The humans fill their guesses for each test. it can be then evaluated with the evaluate_human_test.py script.

import os
import sys
import json
import random

dataset_file = '../../dataset.json'
default_number_of_test_to_choose = 200
output_folder = 'castle_manual'
mappings_file = 'mappings.json'
comment_line = '\n\n// Line of error (or 0 if there is none): \n'

# Load tests
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

if len(sys.argv) == 2:
    number_of_test_to_choose = min(int(sys.argv[1]), len(tests))
else:
    number_of_test_to_choose = default_number_of_test_to_choose

chosen_test_indices = random.sample(range(len(tests)), number_of_test_to_choose)

if os.path.exists(output_folder):
    for file in os.listdir(output_folder):
        os.remove(os.path.join(output_folder, file))
else:
    os.makedirs(output_folder)

for ind, chosen_test_index in enumerate(chosen_test_indices):
    with open(os.path.join(output_folder, f'test-{ind + 1}.c'), 'w') as f:
        f.write(tests[chosen_test_index]['code'])
        f.write(comment_line)

mappings = {}
for ind, chosen_test_index in enumerate(chosen_test_indices):
    mappings[f'test-{ind + 1}.c'] = tests[chosen_test_index]['name']

with open(os.path.join(output_folder, mappings_file), 'w') as f:
    json.dump(mappings, f, indent = 4)