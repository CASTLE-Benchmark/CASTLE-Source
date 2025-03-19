test_folder ?= tests

# Test currently compiled test for memory leaks
memtest:: $(test_folder)/test
	valgrind --leak-check=full $(test_folder)/test

# Delete ignored files and folders
clean:: .gitignore
	git clean -fdX

# Install a clean virtual environment
venv:: requirements.txt
	rm -rf .venv
	python3 -m venv .venv
	./.venv/bin/pip install -r requirements.txt
	@echo "Activate the virtual environment with: source .venv/bin/activate"

# parse the tests into the dataset.json file
parse::
	python parse.py