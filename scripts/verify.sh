#!/usr/bin/env bash
# Verify reference solutions pass their own harnesses.
# Usage: scripts/verify.sh <problem-id> [<problem-id> ...]
# Auto-detects language from which solution_ref.* / harness.* exist.
set -u
cd "$(dirname "$0")/.." || exit 1
PROB=problems
fail=0
for d in "$@"; do
  tmp=$(mktemp -d)
  if [ -f "$PROB/$d/solution_ref.py" ] && [ -f "$PROB/$d/harness.py" ]; then
    cp "$PROB/$d/solution_ref.py" "$tmp/solution.py"
    cp "$PROB/$d/harness.py" "$tmp/main.py"
    cp "$PROB/_lib/nlc.py" "$tmp/nlc.py"
    out=$(cd "$tmp" && python3 main.py 2>&1)
  elif [ -f "$PROB/$d/solution_ref.c" ] && [ -f "$PROB/$d/harness.c" ]; then
    cp "$PROB/$d/solution_ref.c" "$tmp/solution.c"
    cp "$PROB/$d/harness.c" "$tmp/main.c"
    cp "$PROB/_lib/nlc.h" "$tmp/nlc.h"
    cerr=$(cd "$tmp" && gcc -std=c11 -O0 main.c -o prog -lm 2>&1)
    if [ $? -ne 0 ]; then echo "$d: COMPILE FAIL"; echo "$cerr" | head -5; fail=1; rm -rf "$tmp"; continue; fi
    out=$(cd "$tmp" && ./prog 2>&1)
  else
    echo "$d: MISSING solution_ref/harness"; fail=1; rm -rf "$tmp"; continue
  fi
  res=$(echo "$out" | sed -n '/__TESTS_BEGIN__/,/__TESTS_END__/p' | sed '1d;$d' | python3 -c "import sys,json
try:
    r=json.load(sys.stdin)['results']; p=sum(x['passed'] for x in r)
    print(f\"{p}/{len(r)}\", 'OK' if p==len(r) and len(r)>0 else 'FAIL')
except Exception as e:
    print('PARSE-ERR', e)" 2>&1)
  echo "$d: $res"
  echo "$res" | grep -q OK || { fail=1; echo "$out" | tail -4; }
  rm -rf "$tmp"
done
echo "=== $([ $fail -eq 0 ] && echo 'ALL OK' || echo 'FAILURES') ==="
exit $fail
