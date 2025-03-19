import requests
import json

page_size = 100
url = f'https://cloud.codethreat.com/api/scanlog/issues?pageSize={page_size}'
project = 'Test-C'
organization = 'CASTLE-Benchmark'
result_file = 'codethreat.json'


# Get config
with open('secrets.json', 'r') as f:
    secrets = json.load(f)
    token = secrets['token']
    organization = secrets['organization']
headers = {
    'Authorization': f'Bearer {token}',
    'Content-Type': 'application/json',
    'x-ct-organization': organization,
}

# Get first page
print('Downloading findings...')
data = []
first = requests.post(
    url,
    headers=headers,
    data='{}'
).json()
page_size = first['pagination']['page_size']
total_items = first['pagination']['total']
data += first['data']
print('Total items:', total_items)
print('Page size:', page_size)


# Get remaining pages
print('====================')
for i in range(2, total_items//page_size + 2):
    data += requests.post(
        url + f'&page={i}',
        headers=headers,
        data='{}'
    ).json()['data']
    print(f'{len(data)} / {total_items}')
print('Downlad complete')


# Save data
with open(result_file, 'w') as f:
    json.dump(data, f)
print('Data saved to', result_file)