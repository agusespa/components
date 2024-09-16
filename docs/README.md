# Project Overview
Description of the purpose and functionalities.

## Project Structure
Overview of the project's directory structure and key files.

## Teck Stack
LIst the technologies, frameworks and tools used.

# Installation and Setup
Step-by-step guide on how to install and set up the project.

## Prerequisites
List of software and dependencies required.

# Development
Instructions on setting up the project locally for development.

## Workflow
Description of the development process, including branching strategy and code review procedures.
### Version control
See corresponding git/*.md

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

## Coding Standards
Coding conventions and style guides used in the project.

## Testing
Information on how to run tests and details about the testing framework used.

## Deployment
Instructions on how to deploy the application to various environments.

# License
Information about the project's license.
