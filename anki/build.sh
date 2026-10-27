#!/usr/bin/env bash
#
# Build anki/out/FW-Coding-Reps.apkg and anki/out/FW-EE-concept-cards.txt.
#
# Anki.app embeds Python 3.13 and its own `anki` library but ships no python
# executable, so this compiles a tiny launcher against Anki's Python framework
# (once, into anki/.build/) and runs build_decks.py with it. That way the
# package is written by Anki's own exporter. Nothing touches your collection.
#
#   anki/build.sh
#   ANKI_APP=/path/to/Anki.app anki/build.sh
#
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
APP="${ANKI_APP:-/Applications/Anki.app}"
FRAMEWORKS="$APP/Contents/Frameworks"
PYHOME="$(ls -d "$FRAMEWORKS"/Python.framework/Versions/3.* 2>/dev/null | head -1)"
PACKAGES="$APP/Contents/Resources/app_packages"

if [ -z "$PYHOME" ] || [ ! -d "$PACKAGES/anki" ]; then
  echo "error: no bundled Python or anki package found in $APP" >&2
  exit 1
fi

PYVER="$(basename "$PYHOME")"
LAUNCHER="$HERE/.build/python$PYVER"
if [ ! -x "$LAUNCHER" ]; then
  mkdir -p "$HERE/.build"
  printf '#include <Python.h>\nint main(int argc, char **argv) { return Py_BytesMain(argc, argv); }\n' \
    > "$HERE/.build/launcher.c"
  cc -I"$PYHOME/include/python$PYVER" "$HERE/.build/launcher.c" -o "$LAUNCHER" \
    "$PYHOME/Python" -Wl,-rpath,"$FRAMEWORKS"
fi

PYTHONHOME="$PYHOME" PYTHONPATH="$PACKAGES" "$LAUNCHER" "$HERE/build_decks.py"
