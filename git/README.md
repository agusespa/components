# Git workflow (from dev to deploy)
1. Use 'feature' branches for local development.
2. Push and merge into 'develop' branch.
3. Create a new 'release' branch from 'develop' branch.
4. Run the pipeline from the release branch to generate a docker image with the new bundle.

## Branches
#### 'main'
Default and Protected.
Code that has been deployed to PROD and verified.
Only merge into 'main' from 'release' branches. 
#### 'release/'
Read-only. Full deployment pipeline.
A release branch will hold the version of the code to go through the deployment process: DEV -> TEST -> PROD.
Base the release branches on the develop branch and use the module's package version for the branch name: `release/[version]`.
See [versioning](#versioning) section below.
Remove after merging into 'main' branch or if rejected.
#### 'develop'
Code that is ready to be deployed for testing to DEV and TEST environments. Not for local development.
##### 'feature/', 'bugfix/' and other branches
Name them so: '[type]/[issue-number]/[task-title]'
Remove after merging into 'develop' branch.

### Messages
To maintain an explicit commit history that makes it easy to cherrypick, please adhere to  the Conventional Commits Specification. The commit message should be structured as follows:
```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```
Most common types are:
- fix: patches a bug in your codebase (this correlates with PATCH in Semantic Versioning)
- feat: introduces a new feature to the codebase (this correlates with MINOR in Semantic Versioning)
- chore: changes to infrastructure, build processes, documentation, cleanup
- refactor
- build

### Custom Git Hooks
#### Setup
This repository contains custom hooks. To use them (only UNIX), add this to the .git/config file:
```
hooksPath = ./scripts/hooks
```
#### Options
If you need to skip the hook for a specific commit, you can use the `--no-verify` option:
```
git commit -m "message" --no-verify
```
#### Test
See corresponding README.md

## Versioning
This project strictly adheres to Semantic Versioning (SemVer) and it will be checked before merging to release branches.
The script that checks the version is there to ensure that the version is newer, not to check that it is %100 valid. Although it will stop the pipeline if it doesn't adhere to the semver formatting, it won't do so for every case possible. So please review this guidelines before changing it:
- SemVer uses a three-part version number: MAJOR.MINOR.PATCH
MAJOR: Incremented for incompatible, breaking API changes.
MINOR: Incremented for new features that are backwards-compatible.
PATCH: Incremented for backwards-compatible bug fixes.
- The version core is formed of digits 0-9 but must not contain leading zeros. Thus, 2.1.0 is fine but 2.01.0 is not.
- In addition, pre-release versions can be indicated like this:
1.0.0-alpha
1.5.3-beta.2
DO NOT add extra dots ('.') or dashes ('-') to the pre-release version.
In the case of working with divergent branches (such as a previous release for PRE-PROD, and latest for TEST), use the 'pre-release' versioning. PLEASE use snake_case if you need to (for example, 'pre-prod' won't be allowed). Use '1.2.3-preprod.3' or '1.2.3-pre_prod.3' instead.
