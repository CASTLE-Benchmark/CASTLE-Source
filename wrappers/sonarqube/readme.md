# SonarQube

1. Go to sonarcloud.io and log in with GitHub
2. Create an organization, add your username and select the free plan
3. Make the repository public temporarily. It is needed for the free paln
4. Set up the repository, it will start analyzing immediately
5. Click Top right account image > `My Account` > `Security` > enter a name > `Generate Token`
6. Save the token into `secrets.json`:
```json
{
    "sonarqube_api_token": "..."
}
```

7. Update the config in the `download.py`, then: `python download.py`
8. Update the config in the `run.py`, then: `python run.py`
9. Make the repository private again !