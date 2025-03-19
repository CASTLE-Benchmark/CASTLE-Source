# Aidiko

For Aikio, we import the GitHub repository. You need to create a repo with the `repository.py` script, and commit it to a private repository.

1. Log in with GitHub to https://app.aikido.dev
2. Create a workspace and allow access only to the target repository
3. Click `Repositories` > `<your repo name>` > `Start Scan`. Don't forget to start a timer
4. `Actions` > `Export Issues` > `CSV`. Make sure everyting is selected. You can filter it for a given period if you are running the scan on multiple occasions
5. Move the CSV into the current wrapper directory with the name: `issues.csv`
6. `python run.py`
