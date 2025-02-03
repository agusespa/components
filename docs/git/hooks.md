# Custom Git Hooks
## Setup
This repository contains custom hooks. To enable them, add the following line to the .git/config file:
```
hooksPath = ./scripts/hooks
```
## Options
If you need to skip the hook for a specific commit, you can use the `--no-verify` option:
```
git commit -m "message" --no-verify
```
