"""Build the FW Coding Reps Anki package and the concept-card import file.

Run it through build.sh, which uses the Python bundled inside Anki.app, so the
package is written by Anki's own exporter and carries the deck preset.

Outputs (in anki/out/):
  FW-Coding-Reps.apkg       one card per notleetcode rep, in tabs/reps.json order
  FW-EE-concept-cards.txt   30 concept cards for the FW + EE Interview deck

Note GUIDs, the note type id and the preset id are fixed, so importing a newer
build updates the existing cards and preset instead of adding copies.
"""
import html
import json
import os
import re
import tempfile

from anki.collection import Collection, DeckIdLimit, ExportAnkiPackageOptions
from anki.deck_config_pb2 import DeckConfig

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
OUT = os.path.join(HERE, "out")

APP_URL = "http://localhost:5151"
DECK = "FW Coding Reps"
NOTETYPE = "FW Coding Rep"
NOTETYPE_ID = 1793100000001
PRESET_ID = 1793100000002

CONCEPT_DECK = "FW + EE Interview"
CONCEPT_TAGS = "C Firmware Interview NotLeetCode::CodingReps"

FIELDS = ["ProblemId", "Title", "Tier", "Prompt", "Target", "Link", "KeyIdeas", "ClassicBug"]

FRONT = """<div class="rep">
<div class="tier">{{Tier}}</div>
<div class="title">{{Title}}</div>
<div class="prompt">{{Prompt}}</div>
<div class="meta">Target {{Target}} min &middot; blank file &middot; tests must pass</div>
<a class="go" href="{{Link}}">Open the rep in notleetcode &rarr;</a>
</div>"""

BACK = """{{FrontSide}}
<hr id="answer">
<div class="back">
<div class="h">Key ideas</div>
{{KeyIdeas}}
<div class="h">Classic bug</div>
<div class="bug">{{ClassicBug}}</div>
<div class="h">Grade</div>
<table class="rubric">
<tr><td class="g again">Again</td><td>Couldn't pass without the reference, or didn't know the approach</td></tr>
<tr><td class="g hard">Hard</td><td>Passed, but over {{Target}} min or looked something up</td></tr>
<tr><td class="g good">Good</td><td>Passed within {{Target}} min, no lookups</td></tr>
<tr><td class="g easy">Easy</td><td>Passed in under half the target, two runs or fewer</td></tr>
</table>
<div class="note">notleetcode suggests a grade when the tests pass.</div>
</div>"""

CSS = """.card { font-family: -apple-system, "Helvetica Neue", Arial, sans-serif; font-size: 17px;
  line-height: 1.45; color: #1d1d1f; background: #fff; text-align: left; }
.nightMode.card, .night_mode .card { color: #e6e6e6; background: #1e1e1e; }
.rep, .back { max-width: 640px; margin: 0 auto; }
.tier { font-size: 12px; text-transform: uppercase; letter-spacing: .06em; color: #8a8a8e; }
.title { font-size: 26px; font-weight: 650; margin: 4px 0 10px; }
.prompt { font-size: 18px; margin-bottom: 10px; }
.meta { font-size: 14px; color: #8a8a8e; margin-bottom: 16px; }
.go { display: inline-block; padding: 9px 16px; border-radius: 8px; background: #1c7a32;
  color: #fff !important; text-decoration: none; font-weight: 600; }
code { font-family: "SF Mono", Menlo, Consolas, monospace; font-size: .88em;
  background: rgba(127,127,127,.15); padding: 1px 4px; border-radius: 4px; }
.h { font-size: 12px; text-transform: uppercase; letter-spacing: .06em; color: #8a8a8e; margin: 16px 0 4px; }
.back ul { margin: 0; padding-left: 20px; }
.back li { margin: 3px 0; }
.bug { border-left: 3px solid #d64545; padding-left: 10px; }
.rubric { border-collapse: collapse; font-size: 14px; }
.rubric td { padding: 3px 10px 3px 0; vertical-align: top; }
.g { font-weight: 650; }
.again { color: #d64545; } .hard { color: #c98a14; } .good { color: #2e9e4a; } .easy { color: #3d8bfd; }
.note { font-size: 13px; color: #8a8a8e; margin-top: 10px; }"""


def inline(text):
    """Escape HTML, then turn `code` spans into <code> and **bold** into <b>."""
    out = []
    for i, part in enumerate(text.split("`")):
        part = html.escape(part, quote=False)
        if i % 2:
            out.append(f"<code>{part}</code>")
        else:
            out.append(re.sub(r"\*\*(.+?)\*\*", r"<b>\1</b>", part))
    return "".join(out).replace("\n", "<br>")


def load_json(*parts):
    with open(os.path.join(*parts), encoding="utf-8") as f:
        return json.load(f)


def set_fixed_ids(path, notetype_id, preset_id):
    """Give the note type and preset fixed ids so rebuilt packages update in place."""
    col = Collection(path)
    nt = col.models.by_name(NOTETYPE)
    conf = next(c for c in col.decks.all_config() if c["name"] == DECK)
    col.db.execute("update notetypes set id = ? where id = ?", notetype_id, nt["id"])
    col.db.execute("update fields set ntid = ? where ntid = ?", notetype_id, nt["id"])
    col.db.execute("update templates set ntid = ? where ntid = ?", notetype_id, nt["id"])
    col.db.execute("update deck_config set id = ? where id = ?", preset_id, conf["id"])
    col.close()


def add_notetype(col):
    m = col.models.new(NOTETYPE)
    for name in FIELDS:
        col.models.add_field(m, col.models.new_field(name))
    t = col.models.new_template("Rep")
    t["qfmt"], t["afmt"] = FRONT, BACK
    col.models.add_template(m, t)
    m["css"] = CSS
    m["sortf"] = FIELDS.index("Title")
    col.models.add_dict(m)


def add_preset(col):
    conf = col.decks.add_config(DECK)
    conf["new"]["perDay"] = 1
    conf["new"]["delays"] = []
    conf["lapse"]["delays"] = []
    conf["rev"]["perDay"] = 3
    conf["rev"]["maxIvl"] = 180
    conf["desiredRetention"] = 0.85
    conf["newGatherPriority"] = DeckConfig.Config.NEW_CARD_GATHER_PRIORITY_DECK
    conf["reviewOrder"] = DeckConfig.Config.REVIEW_CARD_ORDER_RETRIEVABILITY_ASCENDING
    col.decks.update_config(conf)


def build_reps(workdir):
    reps = load_json(ROOT, "tabs", "reps.json")
    content = load_json(HERE, "rep_cards.json")

    path = os.path.join(workdir, "reps.anki2")
    col = Collection(path)
    add_notetype(col)
    add_preset(col)
    col.close()
    set_fixed_ids(path, NOTETYPE_ID, PRESET_ID)

    col = Collection(path)
    did = col.decks.id(DECK)
    deck = col.decks.get(did)
    deck["conf"] = PRESET_ID
    col.decks.save(deck)
    model = col.models.get(NOTETYPE_ID)

    count = 0
    for n, section in enumerate(reps["sections"], start=1):
        for pid in section["items"]:
            meta = load_json(ROOT, "problems", pid, "meta.json")
            card = content[pid]
            note = col.new_note(model)
            note.guid = f"nlc-rep-{pid}"
            note["ProblemId"] = pid
            note["Title"] = inline(meta["title"])
            note["Tier"] = inline(section["name"])
            note["Prompt"] = inline(card["prompt"])
            note["Target"] = str(reps["targets"][pid])
            note["Link"] = f"{APP_URL}/#rep/{pid}"
            note["KeyIdeas"] = "<ul>" + "".join(f"<li>{inline(i)}</li>" for i in card["ideas"]) + "</ul>"
            note["ClassicBug"] = inline(card["bug"])
            note.tags = [f"FWReps::Tier{n}"]
            col.add_note(note, did)
            count += 1

    out = os.path.join(OUT, "FW-Coding-Reps.apkg")
    col.export_anki_package(
        out_path=out,
        options=ExportAnkiPackageOptions(
            with_scheduling=False, with_deck_configs=True, with_media=False, legacy=False
        ),
        limit=DeckIdLimit(deck_id=did),
    )
    col.close()
    return out, count


def build_concepts():
    cards = load_json(HERE, "concept_cards.json")
    out = os.path.join(OUT, "FW-EE-concept-cards.txt")
    lines = [
        "#separator:tab",
        "#html:true",
        "#notetype:Basic",
        f"#deck:{CONCEPT_DECK}",
        f"#tags:{CONCEPT_TAGS}",
        "#guid column:3",
    ]
    for c in cards:
        front, back = inline(c["front"]), inline(c["back"])
        assert "\t" not in front + back
        lines.append(f"{front}\t{back}\tnlc-concept-{c['id']}")
    with open(out, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return out, len(cards)


def main():
    os.makedirs(OUT, exist_ok=True)
    with tempfile.TemporaryDirectory() as workdir:
        path, n = build_reps(workdir)
    print(f"{n} rep cards -> {os.path.relpath(path, ROOT)}")
    path, n = build_concepts()
    print(f"{n} concept cards -> {os.path.relpath(path, ROOT)}")


if __name__ == "__main__":
    main()
