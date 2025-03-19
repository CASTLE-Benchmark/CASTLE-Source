# Semgrep

1. Register an account (GitHub login is fine)
2. `Settings` > `Tokens` > `API Tokens` > `Create new token`
3. Save the token to `secrets.json`:
```json
{
    "semgrep_token": "..."
}
```
4. Run the evaluator `python run.py`. It will take some time to ramp up
5. The container will be shut down if successful, but won't be deleted. Delete it if you are not running it again.
