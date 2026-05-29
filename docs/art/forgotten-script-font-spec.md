# Forgotten-Script Font — Glyph & Production Spec

> The technical brief for the artist/type designer who builds the [forgotten script](../lore/forgotten-script.md) as a real font. The [cipher mapping](../lore/forgotten-script-cipher.md) is the authoring key (which glyph means which letter); this spec defines the *deliverable*: glyph inventory, codepoint mapping, metrics, the aging styles, OpenType behavior, and Unreal integration. Build to this so art, UI, and localization all draw from one font.

## 1. Render pipeline (why the font must work this way)
In-world text is **stored as plain Latin text** (the `GlyphText` of a `ULoreFragmentDef` / `FLoreFragmentRow`). The font *draws* the botanical glyphs; the codex reveals the plain `TranslatedText` once the player's `RequiredLiteracyTier` is met. So:
- The font is a **1:1 substitution display face**: every Latin letter maps to its cipher glyph. Typing `THOSE` must render the five glyphs for T-H-O-S-E.
- Decode stays deterministic and **localizable** — a translated build swaps the underlying plain text and the same font renders meaningful glyphs, never gibberish.
- The font carries the *look*; the cipher table carries the *meaning*. Keep them in lockstep: one glyph per plain letter, strictly reversible (no shared glyphs).

## 2. Glyph inventory
**31 designed glyphs** + standard support:
- **26 letterforms** — A–Z, per the [cipher motifs](../lore/forgotten-script-cipher.md#2-letter-substitution-cipher-alphabet) (e.g. A = open seed-pod, R = spiral, S = serpentine vine). Botanical, organic, "grown not engineered."
- **5 logograms** — `THE-ENDING`, `IN-MEMORY-OF`, `THE-UNCOUNTED`, `LINEAGE`, `THE-KINDNESS-TAKEN` (see [cipher §3](../lore/forgotten-script-cipher.md#3-logograms-the-lost-concepts)). Each is a single composed glyph, the emotional payload of a literacy reveal.
- **Support:** space, period, comma, hyphen, and digits 0–9 (dates appear on maker's marks and the tally). Digits may be simple tally-style marks.

Lowercase a–z should map to the **same** glyphs as uppercase (case-insensitive cipher) so authoring is forgiving — duplicate the uppercase outlines into the lowercase slots, or alias via `cmap`.

## 3. Codepoint mapping
| Range | Maps to | Notes |
|---|---|---|
| U+0041–U+005A (A–Z) | the 26 letterform glyphs | primary |
| U+0061–U+007A (a–z) | same 26 glyphs | case-insensitive aliasing |
| U+0030–U+0039 (0–9) | tally-style digits | for dates/counts |
| U+0020, U+002E, U+002C, U+002D | space, period, comma, hyphen | punctuation |
| U+E000–U+E004 (PUA) | the 5 logograms, in the order listed in §2 | reached via ligature, see §5 |

## 4. Aging / erosion styles (storytelling requirement)
Older inscriptions are crisp and ornate; the most recent marks (the One Who Counts' late tallies) **degrade toward crude scratches** — the visual history of forgetting *how to write*. Deliver this as a **variable font with a custom `EROS` axis (0–1000)** plus three named instances:
| Instance | EROS | Use |
|---|---|---|
| **Ancient** | 0 | the monument, the First Garden, oldest fragments |
| **Standard** | 500 | general inscriptions, maker's marks |
| **Eroded** | 1000 | the late tally, recent crude marks |

If a variable font is out of scope, ship the three as **separate static `.otf` files** with the same glyph set. The cipher mapping is constant across all three; only styling changes.

## 5. OpenType features
- **`liga`/`dlig` for logograms:** author logograms in plain text as bracketed tags so writers never hand-place PUA codepoints. Map the literal token to its PUA glyph:
  - `[ENDING]` → U+E000, `[INMEMORY]` → U+E001, `[UNCOUNTED]` → U+E002, `[LINEAGE]` → U+E003, `[KINDNESS]` → U+E004.
- **`calt` (contextual alternates):** optional subtle joining/variation so repeated letters don't look mechanically identical (reinforces "grown" feel). Keep deterministic enough that the cipher stays readable.
- **`kern`:** botanical forms will need real kerning to avoid tangling.
- Do **not** use features that change letter *identity* per context — decode must stay 1:1.

## 6. Metrics & technical targets
- **UPM:** 1000. **Cap height:** ~700, **ascender:** ~750, **descender:** ~-250 (generous; organic forms overshoot).
- Heavy strokes — these read at monument scale and at small codex/UI sizes; **hint or test at 14–18px** for the codex body.
- **Format:** OpenType (`.otf`, CFF) or variable `.ttf` for the `EROS` axis. Include `cmap` (format 4 + 12), `GSUB` (liga/calt), `GPOS` (kern).
- **Encoding:** Unicode; no reliance on legacy codepages.

## 7. Deliverables
1. `LongNoonScript-VF.ttf` (variable, `EROS` axis) **or** `LongNoonScript-Ancient/Standard/Eroded.otf`.
2. Editable source: `.glyphs` (Glyphs.app) or UFO + designspace.
3. `glyph-names.md` — glyph ↔ codepoint ↔ cipher-name ↔ motif table (extends [cipher §2/§3](../lore/forgotten-script-cipher.md)).
4. A one-page proof PDF: the alphabet, the 5 logograms, and the two [worked examples](../lore/forgotten-script-cipher.md#4-worked-examples-for-the-team) (monument face; Sealing Spade maker's mark).
5. License grant compatible with commercial Steam release (see [third-party policy](../tech/third-party-plugins.md) if a base/tool font is used).

## 8. Unreal integration (for the engineer)
- Import each file as a **Font** asset under `Content/UI/Fonts/` (Runtime cache for UMG).
- Two font slots in the codex/inscription widgets: **glyph font** (this face) for `GlyphText`, **readable font** (the normal UI face) for `TranslatedText`.
- The codex swaps slot per fragment based on `RequiredLiteracyTier` vs the GameInstance `ScriptLiteracyTier` (see `ULongNoonCodexSubsystem::GetReadableText`), and re-reading auto-upgrades when the tier rises.
- Pick the `EROS` instance per inscription's age (drive it from a future `Inscriptions`/age field, or set per-placed-actor in the greybox).
- The **`[KINDNESS]` logogram** is seeded untranslated across all regions from the start ([easter egg](../lore/easter-eggs.md)); it renders from U+E004 regardless of literacy and only becomes *meaningful* at tier 4.

## 9. Authoring rules (unchanged from the cipher doc)
- Strictly reversible: one glyph per plain letter.
- New logograms only for genuinely lost concepts; never casual words.
- Always author **plain text + `RequiredLiteracyTier`**; never hand-place glyphs — let the font render them, so the literacy-aware reveal stays correct.
