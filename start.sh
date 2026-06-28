#!/usr/bin/env bash
#
# Start the notleetcode app.
#
#   ./start.sh           # start on http://localhost:5151
#   PORT=8080 ./start.sh # start on a different port
#   ./start.sh --open    # also open the app in your browser
#
set -euo pipefail

cd "$(dirname "$0")"

PORT="${PORT:-5151}"
URL="http://localhost:${PORT}"

# --- check required tools ---
command -v node >/dev/null 2>&1 || { echo "error: node is not installed (https://nodejs.org)"; exit 1; }
command -v python3 >/dev/null 2>&1 || echo "warning: python3 not found — Python problems won't run"
command -v gcc >/dev/null 2>&1 || command -v cc >/dev/null 2>&1 || echo "warning: no C compiler (gcc/cc) found — C problems won't run"

# --- open browser once the server is up (optional) ---
if [ "${1:-}" = "--open" ]; then
  (
    for _ in $(seq 1 50); do
      if curl -s -o /dev/null "$URL" 2>/dev/null; then break; fi
      sleep 0.1
    done
    case "$(uname)" in
      Darwin) open "$URL" ;;
      *)      xdg-open "$URL" >/dev/null 2>&1 || true ;;
    esac
  ) &
fi

echo "starting notleetcode on ${URL}  (Ctrl+C to stop)"
exec env PORT="$PORT" node server.js
