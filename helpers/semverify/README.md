Scripts to be used as part of a bash script in CI pipelines. 

To verify that a string adheres to semver formatting:
```bash
if ! python3 ./scripts/semverify.py "$VERSION"; then 
    echo "ERROR: invalid version"
    exit 1
fi
```

To verify that a pair of semver corresponds to a valid progression:
```bash
if ! python3 ../scripts/semverify.py "$TARGET_VERSION" "$MERGE_REQUEST_VERSION"; then 
    echo "ERROR: Merge request version ($MERGE_REQUEST_VERSION) must be valid and higher than target branch version ($TARGET_VERSION)"
    exit 1
fi
```
