clang-format-7 --version
echo Validating code formatting.
clang-format-7 --style=file -i src/*.{h,cpp} src/*/*.{h,cpp} tools/*/*.{h,cpp} xuocore/*.{h,cpp} common/*/*.{h,cpp}
dirty=$(git ls-files --modified)
if [[ $dirty ]]; then
    echo Failed. The following files are incorrectly formatted:
    echo $dirty
    echo "Diff:"
    git --no-pager diff --no-color
    exit 1
else
    echo Code formatting validation passed.
fi
docker run -v ${GITHUB_ACTION_PATH}:/data fsfe/reuse:latest lint
if [[ $? != 0 ]]; then
    echo See above output to find the issue.
    echo Failed due missing copyright or licensing information.
    exit 1
fi