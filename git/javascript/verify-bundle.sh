#!/bin/sh

# Script used as verification step in git hook or CI pipeline for javascript projects

set -e
set -x

# Verify bundling
npm ci
npm run lint-quiet || (echo "ERROR: Lint failed" && exit 1)
npm run test || (echo "ERROR: Tests failed" && exit 1)
npx tsc --noemit || (echo "ERROR: Type check failed" && exit 1)

rollup_output=$(npx rollup -c 2>&1)
if echo "$rollup_output" | grep -q "!"; then
    echo "ERROR: Bundling contains errors"
    exit 1
fi

# GitLab Job for checking version
if [ $CI_MERGE_REQUEST_TARGET_BRANCH_NAME = "main" -o $CI_MERGE_REQUEST_TARGET_BRANCH_NAME = "develop" ] ; then
    git fetch origin $CI_MERGE_REQUEST_TARGET_BRANCH_NAME
    git checkout $CI_MERGE_REQUEST_TARGET_BRANCH_NAME
    TARGET_VERSION=$(grep -E '"version":' package.json | awk -F '"' '{print $4}')

    git checkout $CI_COMMIT_SHA
    MERGE_REQUEST_VERSION=$(grep -E '"version":' package.json | awk -F '"' '{print $4}')

    # Point to helper function
    if ! python3 ./semver.py "$TARGET_VERSION" "$MERGE_REQUEST_VERSION"; then 
        echo "ERROR: Merge request version ($MERGE_REQUEST_VERSION) must be valid and higher than target branch version ($TARGET_VERSION)"
        exit 1
    fi
fi
