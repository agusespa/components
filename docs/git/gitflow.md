# GitFlow Development Workflow

This project follows the **GitFlow** branching model, which structures development using separate branches for features, releases, and production deployments. 
Below are the guidelines for contributing and deploying.

## Branching Strategy

Gitflow defines specific branch types:
- `main` – The stable branch containing production-ready code.
- `develop` – The integration branch where features are merged before release.
- `feature/*` – Used for developing new features.
- `release/*` – Used for preparing a release.
- `hotfix/*` – Used for critical fixes in production.

## Development Workflow
1. **Branch off from `develop`**
   ```sh
   git switch develop
   git pull origin develop
   git switch -c feature/your-feature-name
   ```
2. **Work on your changes**
   ```sh
   git add .
   git commit -m "feat: implement new feature"
   ```
3. **Rebase onto the latest `develop` before merging**
   ```sh
   git switch develop
   git pull origin develop
   git switch feature/your-feature-name
   git rebase develop
   ```
4. **Push and create a Merge/Pull Request (PR)**
   ```sh
   git push origin feature/your-feature-name
   ```
   Ensure all automated tests pass before requesting a review.
   A reviewer must approve the PR before merging.
   
5. **Merge the branch into `develop`**
   Once approved, merge the branch into `develop` (squash recommended to keep a clean history).
   Delete the feature branch after merging.

## Deployment Process
### Staging Deployment (Manual)
Once `develop` is stable and ready for release, create a release branch:
   ```sh
   git switch develop
   git pull origin develop
   git switch -c release/x.y.z
   ```
Run the CI/CD deployment to staging environment.
### Production Deployment (Manual)
Once staging is verified, a **manual deployment** to production is required.
To deploy to production, merge the release branch into `main` and create a Git tag for the release:
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
- Release to staging and verify.
- Follow production deployment process.
- Finaly merge into `develop`.
