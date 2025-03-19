# CodeQL

GitHub's code scanning utility language and library

1. Build the global builder image if not already `pushd ../../ && ./build.sh && popd`
2. Extract the tests to the `tests` folder `python prepare.py`
3. Build up the CodeQL container `./build.sh`
4. Set up the database
```bash
docker run --rm --platform linux/amd64 --entrypoint=codeql -it -v $(pwd)/tests:/root/home/tests castle-wrapper-codeql database create codeqldb --language=cpp --overwrite
```
5. Start the container
```bash
docker run --rm --platform linux/amd64 --entrypoint=bash -it -v $(pwd)/tests:/root/home/tests castle-wrapper-codeql
```
5. Run the command within the container to start the evaluation:
```bash
codeql database analyze codeqldb /root/home/codeql-repo/cpp/ql/src/codeql-suites/cpp-security-extended.qls --format=sarif-latest --output=result-security-extended.sarif --ram 6000
```
| I set the RAM to be around 6Gb, because it crashed in some tests. My laptop still did not have to use swap memory, but it was crashing even at 5Gb. Might be a bug with CodeQL because the laptop had more than enough RAM. It's weird that it doesn't just skip checks where it runs over its self-imposed RAM limit on.
6. The result is saved at `tests/result-security-extended.sarif`. Process it into the final report using `python run.py`

## References

- https://codeql.github.com/docs/codeql-overview/supported-languages-and-frameworks/
- https://docs.github.com/en/code-security/codeql-cli/using-the-advanced-functionality-of-the-codeql-cli/advanced-setup-of-the-codeql-cli