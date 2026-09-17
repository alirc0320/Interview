#!/usr/bin/env bash
# Formats all C++ source/header files in the repo in place using clang-format.
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
if ! command -v "$CLANG_FORMAT" >/dev/null 2>&1; then
    XCODE_CF="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang-format"
    if [ -x "$XCODE_CF" ]; then
        CLANG_FORMAT="$XCODE_CF"
    else
        echo "clang-format not found. Install it with: brew install clang-format" >&2
        exit 1
    fi
fi

files=()
while IFS= read -r -d '' f; do
    files+=("$f")
done < <(find . -type d \( -name build -o -name .git \) -prune -o \
    -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0)

if [ ${#files[@]} -eq 0 ]; then
    echo "No C++ files found."
    exit 0
fi

"$CLANG_FORMAT" -i "${files[@]}"
echo "Formatted ${#files[@]} file(s)."
