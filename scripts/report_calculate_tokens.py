# Calculates the tokens used for running a report with an LLM

import json
import sys
import tiktoken

dataset_file = 'dataset.json'
llm_tokenizer_encoding = 'cl100k_base'
reports_folder = 'reports'
report_suffix = '-report.json'

# Load the report
report = None
if len(sys.argv) < 2:
    print("Usage: python scripts/report_calculate_tokens.py <report-name>")
    sys.exit(1)
report_name = sys.argv[1]
report_file = f"{reports_folder}/{report_name}{report_suffix}"
with open(report_file, 'r') as f:
    report = json.load(f)
tests = report['tests']

# Load the dataset
dataset = None
with open(dataset_file, 'r') as f:
    dataset = json.load(f)
    
# Calculate the output tokens
llm_tokenizer = tiktoken.get_encoding(llm_tokenizer_encoding)
output_tokens = 0
for test in tests:
    output_tokens += len(llm_tokenizer.encode(test['report']))
    
print(f"Input tokens: {dataset['total_llm_input_tokens']}")
print(f"Output tokens: {output_tokens}")