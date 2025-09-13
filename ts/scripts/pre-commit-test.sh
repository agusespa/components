#!/bin/bash

echo "Testing file filtering..."

# Simulate git diff output
STAGED_FILES="src/app.js
src/component.tsx
src/styles.css
src/config.json
README.md
package.json"

# Test JS/TS filtering
JS_FILES=$(echo "$STAGED_FILES" | grep -E '\.(js|jsx|ts|tsx|json)$' || true)
echo "JS/TS files found:"
echo "$JS_FILES"

# Test CSS filtering  
CSS_FILES=$(echo "$STAGED_FILES" | grep -E '\.(scss|css)$' || true)
echo "CSS files found:"
echo "$CSS_FILES"

# Test test file discovery
mkdir -p test-structure/src/utils/__tests__
touch test-structure/src/utils/__tests__/helper.test.js

TEST_FILES=$(echo "src/utils/helper.js" | while read -r file; do
    dir=$(dirname "$file")
    filename=$(basename "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"
    test_file="test-structure/$dir/__tests__/$filename.test.$extension"
    [ -e "$test_file" ] && echo "$test_file"
done)

echo "Test files found:"
echo "$TEST_FILES"

rm -rf test-structure
echo "✅ Logic test completed!"
