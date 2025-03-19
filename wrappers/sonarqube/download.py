import requests
import json

page_size = 100
organization = 'castle-benchmark' # Only lowercase! The same as in the URL
url = f'https://sonarcloud.io/api/issues/search?organization={organization}&p='
api_token_name = 'sonarqube_api_token'
result_file = 'sonarqube.json'


# Get config
with open('secrets.json', 'r') as f:
    secrets = json.load(f)
    token = secrets[api_token_name]
headers = {
    'Authorization': f'Bearer {token}',
}


# Get first page
print('Getting data...')
data = []
first = requests.get(
    url + '1',
    headers=headers,
).json()
page_size = first['paging']['pageSize']
total_items = first['paging']['total']
data += first['issues']
print('Total items:', total_items)
print('Page size:', page_size)


# Get remaining pages
print('====================')
for i in range(2, total_items//page_size + 2):
    data += requests.post(
        url + f'{i}',
        headers=headers,
    ).json()['issues']
    print(f'{len(data)} / {total_items}')
print('Downlad complete')


# Save data
with open(result_file, 'w') as f:
    json.dump(data, f)
print('Data saved to', result_file)