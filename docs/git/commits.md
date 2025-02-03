# Commit Messages
To maintain an explicit commit history that makes it easy to cherrypick, please adhere to  the Conventional Commits Specification. 
The commit message should be structured as follows:
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
