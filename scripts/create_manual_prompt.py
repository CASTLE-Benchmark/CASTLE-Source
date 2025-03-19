# Create a file with the preprompt and the code snippets for easy copy-pasting
# Used for models where the API is prohibitively expensive to use
# Usage: python scripts/create_manual_prompt.py

import json
import tiktoken


dataset_file = 'dataset.json'
prompt_file = 'prompt.txt'
output_file = 'manual_prompts.md'
llm_tokenizer = tiktoken.get_encoding('cl100k_base')


# Read data
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
tests = dataset['tests']
    
with open(prompt_file, 'r') as f:
    prompt = f.read()
    
# Calculate prepromt token size
preprompt_size = len(llm_tokenizer.encode(prompt))
total_size = preprompt_size * len(tests)

# Write tests
with open(output_file, 'w') as f:
    for t in tests:
        code_snippet = t['code']
        markdown_code = f'```c\n{code_snippet}\n```'
        total_size += len(llm_tokenizer.encode(markdown_code))
        
        f.write(f'# ========================== {t["id"]} ==========================\n\n{prompt}{code_snippet}\n\n')

# Stats
print(f'Tests: {len(tests)}')
print(f'Preprompt size: {preprompt_size}')
print(f'Total size: {total_size}')
print(f'Output: {output_file}')