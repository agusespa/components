# Trunk-Based Development Workflow

This project follows a **Trunk-Based Development (TBD)** workflow, where all changes are integrated into the `main` branch frequently, ensuring a continuously deployable state. Below are the guidelines for contributing and deploying.

## Branching Strategy
The `main` branch is the **single source of truth** and should always be in a deployable state.
Developers create **short-lived feature branches** from `main` for their changes.
All merges into `main` trigger an **automatic testing and deployment to the staging environment**.

## Development Workflow
1. **Branch off from `develop`**
   ```sh
   git switch main
   git pull origin main
   git switch -c feature/my-new-feature
   ```
2. **Work on your changes and commit frequently**
   ```sh
   git add .
   git commit -m "feat: implement new feature"
   ```
3. **Rebase onto the latest `main` before merging**
   ```sh
   git switch main
   git pull origin main
   git switch feature/my-new-feature
   git rebase main
   ```
4. **Push and create a Merge/Pull Request (PR)**
   ```sh
   git push origin feature/my-new-feature
   ```
   Ensure all automated tests pass before requesting a review.
   A reviewer must approve the PR before merging.
   
5. **Merge the branch into `main` (Squash & Merge recommended)**
   Once approved, merge the branch into `main` (squash recommended to keep a clean history).
   Delete the feature branch after merging.
   
## Deployment Process
### Staging Deployment (automated)
Every merge into `main` **automatically deploys to the staging environment**.
CI/CD runs tests and deploys the latest code to staging.
Developers and testers verify changes in the staging environment.
### Production Deployment (Manual)
Once staging is verified, a **manual deployment** to production is required.
To deploy to production, create a Git tag for the release:
  ```sh
  git tag v1.0.0
  git push origin v1.0.0
  ```
This triggers a deployment pipeline that releases the tagged version to production.

## Handling Hotfixes
- For urgent fixes in production:
  ```sh
  git switch main
  git pull origin main
  git switch -c hotfix/x.y.z-fix
  ```
- After applying the fix:
  ```sh
  git commit -m "Hotfix: Describe the fix"
  git push origin hotfix/x.y.z-fix
  ```
- Merge into `main`, verify and tag for production deployment.
