#!/usr/bin/env python3
"""Daily FW Coding Reps nudge: a macOS notification if no rep was graded today.

Reads the Anki collection read-only (it never writes to it) and stays silent
when the FW Coding Reps deck isn't imported yet, when a card in it was already
reviewed today, or when nothing is due. Otherwise it names the shortest due
card by its target time, or the next new card.

Installed by install_nudge.sh as a launchd job. Runs under the system
python3, so keep it Python 3.9 compatible and free of third-party imports.

  python3 nudge.py --dry-run    print the notification instead of showing it
  python3 nudge.py --force      ignore whether a rep was already done today
"""
import datetime
import glob
import html
import json
import os
import re
import shutil
import sqlite3
import subprocess
import sys
import tempfile
import time

ANKI_BASE = os.environ.get("ANKI_BASE", os.path.expanduser("~/Library/Application Support/Anki2"))
DECK = "FW Coding Reps"
LEARN_AHEAD_SECS = 20 * 60


def unicase(a, b):
    a, b = a.casefold(), b.casefold()
    return (a > b) - (a < b)


def connect(path, tmp):
    try:
        db = sqlite3.connect(f"file:{path}?mode=ro", uri=True)
        db.execute("select count(*) from col").fetchone()
    except sqlite3.Error:
        # Fall back to reading a copy (with its write-ahead log) if Anki holds the file.
        for suffix in ("", "-wal"):
            if os.path.exists(path + suffix):
                shutil.copy2(path + suffix, os.path.join(tmp, "c.anki2" + suffix))
        db = sqlite3.connect(os.path.join(tmp, "c.anki2"))
    db.create_collation("unicase", unicase)
    return db


def rollover_hour(db):
    row = db.execute("select val from config where key = 'rollover'").fetchone()
    try:
        return int(json.loads(row[0])) if row else 4
    except (TypeError, ValueError):
        return 4


def scheduler_day(ts, rollover):
    return (datetime.datetime.fromtimestamp(ts) - datetime.timedelta(hours=rollover)).date()


def plain(field):
    return html.unescape(re.sub(r"<[^>]+>", "", field)).strip()


def check(db):
    deck = db.execute("select id from decks where name = ?", (DECK,)).fetchone()
    if not deck:
        return None
    did = deck[0]
    rollover = rollover_hour(db)
    crt = db.execute("select crt from col").fetchone()[0]
    now = time.time()
    today = (scheduler_day(now, rollover) - scheduler_day(crt, rollover)).days
    day_start = datetime.datetime.combine(scheduler_day(now, rollover), datetime.time(rollover))
    day_start_ms = int(day_start.timestamp() * 1000)

    done = db.execute(
        "select count(*) from revlog r join cards c on c.id = r.cid where c.did = ? and r.id >= ?",
        (did, day_start_ms),
    ).fetchone()[0]

    rows = db.execute(
        "select c.queue, c.due, n.mid, n.flds from cards c join notes n on n.id = c.nid "
        "where c.did = ? and c.queue >= 0",
        (did,),
    ).fetchall()
    names = {}
    due, new = [], []
    for queue, card_due, mid, flds in rows:
        if mid not in names:
            names[mid] = [r[0] for r in db.execute(
                "select name from fields where ntid = ? order by ord", (mid,))]
        fields = dict(zip(names[mid], flds.split("\x1f")))
        title = plain(fields.get("Title", "a rep"))
        try:
            target = int(plain(fields.get("Target", "")))
        except ValueError:
            target = 999
        if (queue in (2, 3) and card_due <= today) or (queue == 1 and card_due <= now + LEARN_AHEAD_SECS):
            due.append((target, title))
        elif queue == 0:
            new.append((card_due, title, target))
    return {"done": done, "due": sorted(due), "new": min(new) if new else None}


def message(state):
    if state["due"]:
        target, title = state["due"][0]
        n = len(state["due"])
        return (f"{n} rep{'s' if n != 1 else ''} due", f"Shortest: {title} ({target} min)")
    if state["new"]:
        _, title, target = state["new"]
        return ("New rep ready", f"{title} ({target} min)")
    return None


def notify(subtitle, body):
    def q(s):
        return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'
    script = f"display notification {q(body)} with title {q(DECK)} subtitle {q(subtitle)}"
    subprocess.run(["osascript", "-e", script], check=False)


def main(argv):
    dry_run, force = "--dry-run" in argv, "--force" in argv
    for path in sorted(glob.glob(os.path.join(ANKI_BASE, "*", "collection.anki2"))):
        tmp = tempfile.mkdtemp()
        try:
            db = connect(path, tmp)
            try:
                state = check(db)
            finally:
                db.close()
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
        if state is None:
            continue
        if state["done"] and not force:
            print(f"{path}: rep already done today")
            return 0
        msg = message(state)
        if not msg:
            print(f"{path}: nothing due")
            return 0
        print(f"{path}: {msg[0]} - {msg[1]}")
        if not dry_run:
            notify(*msg)
        return 0
    print(f"{DECK} not found in any Anki profile")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
