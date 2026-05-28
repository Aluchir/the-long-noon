# The Forgotten Script — language design

> The pre-Long-Noon written language. It is the game's primary mechanism for delivering buried truth: unreadable at the start, gradually learned by the player, fully legible by the end. Writing survived where memory didn't, because writing is memory placed outside the mind.

## 1. Design goals
- **Diegetic progression of literacy.** The player can't read it at first; by the end they can. Reading *is* a progression system, parallel to gear tiers.
- **Retroactive revelation.** Each literacy gain reaches *backward* — text seen earlier becomes readable, recontextualizing earlier regions (the Sunhollow inscription pays off in Act 3).
- **One unifying thread.** The monument, the tally's dedication, the machine schematics, the true plant-names, and the Gardener's own words are *all the same script.* Learning it ties every lore thread into one.
- **Optional depth, not a wall.** Critical-path understanding is delivered through experience and partial reads; *full* literacy rewards the thorough without gating the casual.

## 2. How the player learns it (literacy tiers)
| Tier | Where | What unlocks |
|---|---|---|
| 0 — Illiterate | Sunhollow (R1) | Script appears as beautiful, unreadable marks. First fragment filed in codex. |
| 1 — Glyphs | Overgrowth (R2) | Context-matched words (true plant-names beside drawings). Player learns nouns. |
| 2 — Phrases | Stillworks (R3) | A recovered "script-key" unlocks short sentences (memorial dedications, machine labels). |
| 3 — Passages | The Hush (R4) | Final fragments complete grammar; longer texts readable. |
| 4 — Fluent | First Garden (R5) | The Gardener's own words are legible; all earlier text retroactively clear. |

Each tier is earned through **lore fragments** (see [codex catalogue](codex-catalogue.md)), not XP — you learn the language by finding enough of it.

## 3. Linguistic flavor (for writers & artists)
- **Tone of the language:** warm, ceremonial, a little archaic — a tongue that had many words for *ending, passing, remembrance, lineage,* concepts the present world has lost. Its richest vocabulary is exactly what the post-Noon world can no longer feel.
- **A telling gap:** the script has **no word the present uses for "death" casually** — it has reverent, specific terms (the way a culture that took death seriously would). Conversely, the *present* spoken language has almost no death-vocabulary at all. The contrast is a lore clue in itself.
- **The Gardener's word:** the Gardener has its own glyph for what it removed. The closest translation is **"the kindness I took away."** When the player can finally read it, that single glyph lands the whole tragedy.

## 4. Visual / art direction for the script
- Flowing, organic, botanical letterforms — fitting a culture and a world entangled with growing things. Should feel *grown,* not engineered.
- Consistent glyph set so observant players can start decoding before the game formally teaches them (rewards attention).
- **Erosion as storytelling:** older inscriptions are crisp and complex; the most recent marks (e.g. the One Who Counts' late tallies) degrade toward crude scratches — the visual history of forgetting *how to write.*
- Must be legible enough to localize: design a real, consistent cipher so translated builds can render meaningful (not gibberish) text.

## 5. Implementation notes
- Treat literacy as a **player stat with tiers**; gate which strings render as "translated" vs "glyphs" by current tier.
- Store each in-world text as `{glyphs, translation, requiredTier}`; the UI reveals the translation once the tier is met, and **re-reading earlier texts auto-upgrades** to show the now-readable version.
- The codex logs every encountered text with its current readability, so players can revisit and watch the world become legible.
- Build the cipher as a real font + substitution table so art, UI, and localization all draw from one source of truth.

## 6. Threads that converge on the script
- **The monument** (Keeper) — its inscription is the emotional payoff of literacy.
- **The tally's dedication** (One Who Counts) — *"so that no one goes uncounted."*
- **Machine schematics** (Stillworks) — the lost functions of ending.
- **True plant-names** (Forager) — nouns, the first foothold.
- **The Gardener's words** (First Garden) — the final, fluent revelation.
- **The Rememberer's archive** (Hush) — the only fully-written intact history; the ultimate reading reward.
