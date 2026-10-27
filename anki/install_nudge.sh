#!/usr/bin/env bash
#
# Install (or remove) the daily FW Coding Reps nudge as a launchd job.
#
#   anki/install_nudge.sh              install, notify at 19:00
#   NUDGE_HOUR=20 anki/install_nudge.sh
#   anki/install_nudge.sh --uninstall
#
# The script is copied to ~/Library/Application Support/notleetcode/ because
# launchd jobs can't read ~/Documents without a privacy prompt. If the Mac is
# asleep at the set time, launchd runs the job when it wakes.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
LABEL="com.notleetcode.rep-nudge"
DEST="$HOME/Library/Application Support/notleetcode"
PLIST="$HOME/Library/LaunchAgents/$LABEL.plist"
LOG="$HOME/Library/Logs/notleetcode-rep-nudge.log"
DOMAIN="gui/$(id -u)"

launchctl bootout "$DOMAIN/$LABEL" 2>/dev/null || true

if [ "${1:-}" = "--uninstall" ]; then
  rm -f "$PLIST" "$DEST/rep_nudge.py"
  echo "removed $LABEL"
  exit 0
fi

HOUR="${NUDGE_HOUR:-19}"
MINUTE="${NUDGE_MINUTE:-0}"

mkdir -p "$DEST" "$(dirname "$PLIST")"
cp "$HERE/nudge.py" "$DEST/rep_nudge.py"

cat > "$PLIST" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>Label</key><string>$LABEL</string>
  <key>ProgramArguments</key>
  <array>
    <string>/usr/bin/python3</string>
    <string>$DEST/rep_nudge.py</string>
  </array>
  <key>StartCalendarInterval</key>
  <dict>
    <key>Hour</key><integer>$HOUR</integer>
    <key>Minute</key><integer>$MINUTE</integer>
  </dict>
  <key>StandardOutPath</key><string>$LOG</string>
  <key>StandardErrorPath</key><string>$LOG</string>
</dict>
</plist>
EOF

launchctl bootstrap "$DOMAIN" "$PLIST"
printf 'installed %s: daily at %02d:%02d (log: %s)\n' "$LABEL" "$HOUR" "$MINUTE" "$LOG"
