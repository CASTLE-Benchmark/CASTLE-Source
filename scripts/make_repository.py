# Make a repository folder with the tests and a Makefile.
# The tests are loaded from the dataset.json file.
# This is used to create a folder for some tools, as some work by uploading a repository.

import json
import os

repository_folder = 'repository'
dataset_file = 'dataset.json'
makefile_name = 'Makefile'
makefile = 'all: '

# Create or clear the repository folder
if not os.path.exists(repository_folder):
    os.makedirs(repository_folder)
else:
    for file in os.listdir(repository_folder):
        os.remove(os.path.join(repository_folder, file))


# Load the dataset
with open(dataset_file, 'r') as file:
    dataset = json.load(file)
tests = dataset['tests']


# Save the tests in the repository folder
names = []
for test in tests:
    test_name = test['name'] # CASTLE-22-1.c
    names.append(test_name)
    test_file = os.path.join(repository_folder, test_name)
    with open(test_file, 'w') as file:
        file.write(test['code'])
    print(f'Saved: {test_file}')
    
# Create the makefile
makefile += ' '.join([name.replace('.c', '') for name in names]) + '\n\n'
for test in tests:
    makefile += f'{test["name"].replace(".c", "")}: {test["name"]}\n\t{test["compile"].replace("-fno-stack-protector -fno-pie -no-pie -z execstack", "")}\n\n'
with open(os.path.join(repository_folder, makefile_name), 'w') as f:
    f.write(makefile)
print(f'Wrote Makefile')

print('Done')
