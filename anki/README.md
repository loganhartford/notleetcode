# FW Coding Reps: Anki deck and daily nudge

Anki schedules the reps; notleetcode is where you write them. Each card is one
problem from `tabs/reps.json`, solved from a blank file every time it comes due.

## Files

| File | What it is |
|------|------------|
| `rep_cards.json` | Card text per problem: one-line prompt, key ideas, classic bug |
| `concept_cards.json` | 30 concept cards for the FW + EE Interview deck |
| `build_decks.py` | Builds both imports with Anki's own library |
| `build.sh` | Runs `build_decks.py` with the Python bundled in Anki.app |
| `nudge.py` | Daily notification if no rep was graded today |
| `install_nudge.sh` | Installs or removes the nudge as a launchd job |

Card order, tiers and target times come from `tabs/reps.json`; titles come from
each problem's `meta.json`.

## Build

```sh
anki/build.sh
```

This writes two files to `anki/out/` (not committed):

- `FW-Coding-Reps.apkg`: 58 cards in a top-level **FW Coding Reps** deck, plus its preset.
- `FW-EE-concept-cards.txt`: 30 Basic cards for **FW + EE Interview**, tagged `NotLeetCode::CodingReps`.

`build.sh` needs Anki.app in `/Applications` (or set `ANKI_APP`) and a C compiler.
It never opens your collection.

## Import

In Anki: **File → Import**.

1. `anki/out/FW-Coding-Reps.apkg`: tick **Import any deck presets**. Leave
   **Merge note types** off.
2. `anki/out/FW-EE-concept-cards.txt`: the file's headers already pick the
   Basic note type, the FW + EE Interview deck and the tags; just click Import.

Re-importing a newer build updates the existing cards and the preset in place.
Note GUIDs, the note type id and the preset id are fixed.

The **FW Coding Reps** preset:

| Setting | Value |
|---------|-------|
| New cards/day | 1 |
| Maximum reviews/day | 3 (new cards only appear when reviews leave room) |
| Learning and relearning steps | none |
| Desired retention (FSRS) | 0.85 |
| Maximum interval | 180 days |
| New card gather order | Deck (tier order) |
| Review sort order | Retrievability, ascending |

## Doing a rep

1. Anki shows the card front. Click **Open the rep in notleetcode** (the server
   must be running on port 5151).
2. Solve it from the blank starter until the tests pass, or give up.
3. notleetcode suggests a grade. Flip the card and grade it in Anki.

## Daily nudge

```sh
anki/install_nudge.sh                  # notify at 19:00
NUDGE_HOUR=20 anki/install_nudge.sh    # another time
anki/install_nudge.sh --uninstall
python3 anki/nudge.py --dry-run        # print what it would say now
```

At the set time, if no FW Coding Reps card was reviewed today, a macOS
notification names the shortest due card by target time, or the next new card.
It reads the collection read-only and stays silent until the deck is imported.
The notification comes from Script Editor (osascript), so macOS may ask once
whether to allow its notifications. Its log is
`~/Library/Logs/notleetcode-rep-nudge.log`.
