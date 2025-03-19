import os
import json


tests_folder = 'tests'
makefile_name = 'Makefile'
makefile = 'all: '


# Load tests
with open('../../dataset.json', 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']

# Create makefile all command
names = [ test['name'] for test in tests ]
makefile += ' '.join([name.replace('.c', '') for name in names]) + '\n\n'

# Write test files
os.mkdir(tests_folder)
for test in tests:
    with open(os.path.join(tests_folder, test['name']), 'w') as f:
        f.write(test['code'])
    makefile += f'{test["name"].replace(".c", "")}: {test["name"]}\n\t{test["compile"].replace("-fno-stack-protector -fno-pie -no-pie -z execstack", "")}\n\n'
print(f'Wrote {len(tests)} test files')

# Write makefile
with open(os.path.join(tests_folder, makefile_name), 'w') as f:
    f.write(makefile)
print(f'Wrote Makefile')
