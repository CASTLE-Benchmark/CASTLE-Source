# LLM

Shared wrapper for running any LLM with OpenAI-compliant API.

1. Register for the service. We use OpenAI for GPTs, and OpenRouter for everything else
2. Generate an API token and save it into `secrets.json`
```json
{
  "api_key": "..."
}
```

3. Edit the model information, and other settings in `run.py`