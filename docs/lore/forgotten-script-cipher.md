# Forgotten Script — Cipher Mapping

> The concrete substitution the [forgotten script](forgotten-script.md) uses. A real, consistent cipher so attentive players can decode words before the game teaches the tier, so inscriptions render meaningfully, and so localization stays possible. This is the authoring key; the in-game *look* (organic, botanical letterforms) is an art task on top of this mapping.

## 1. How it works
- The script is a **1:1 glyph substitution** over the Latin alphabet (a real cipher, not random scribble), plus a small set of **logograms** for the world's load-bearing lost concepts.
- Render pipeline: store each in-world string as plain text; a font/shaper draws the glyphs. The codex reveals the plain text once the player's literacy tier is high enough (see [forgotten-script.md](forgotten-script.md) and the `ULoreFragmentDef` `RequiredLiteracyTier`).
- **Letterform note:** glyphs grow more ornate the older the inscription; recent marks (the tally) degrade toward crude scratches. The cipher mapping below is constant; only the rendered styling ages.

## 2. Letter substitution (cipher alphabet)
A reversible monoalphabetic mapping. Plain letter -> spoken cipher-name (used for the wordless vocalization cadence) -> rough glyph description for the artist.

| Plain | Cipher name | Glyph motif |
|---|---|---|
| A | ah | open seed-pod |
| B | bel | double leaf |
| C | cir | curl / tendril |
| D | dun | rooted stem |
| E | eth | three-veined leaf |
| F | fen | fern frond |
| G | gol | coiled shoot |
| H | hal | paired stalks |
| I | ith | single reed |
| J | jor | hooked vine |
| K | kel | branching fork |
| L | lun | drooping bloom |
| M | mor | twin roots |
| N | nor | knotted stem |
| O | oth | ring / closed bud |
| P | pel | budding node |
| Q | quth | thorned ring |
| R | reth | spiral |
| S | sel | wave / serpentine vine |
| T | tor | tall stalk crossbar |
| U | urn | cupped petal |
| V | ven | split frond |
| W | wel | triple stalk |
| X | xal | crossed thorns |
| Y | yth | forked reed |
| Z | zel | zigzag creeper |

> Implementation: ship this as a TrueType/OpenType font where each Latin codepoint draws its glyph motif. The "cipher name" column drives the NPC vocalization rhythm so spoken gibberish maps consistently to written text.

## 3. Logograms (the lost concepts)
Single glyphs for ideas the post-Long-Noon world no longer has words for. These are the emotional payload when the player can finally read them.

| Logogram | Meaning | First readable (tier) | Appears on |
|---|---|---|---|
| **THE-ENDING** | death / to end (reverent, not casual) | 2 | machine plates, memorial dedication |
| **IN-MEMORY-OF** | the dedicatory phrase | 2 | the monument inscription |
| **THE-UNCOUNTED** | "so that no one goes uncounted" | 3 | the Hall of Marks dedication |
| **LINEAGE** | family / those who come after | 3 | swallowed-home fragments |
| **THE-KINDNESS-TAKEN** | the Gardener's own glyph for what it removed | 4 | the First Garden; hidden everywhere |

> The **THE-KINDNESS-TAKEN** logogram is seeded, untranslated, in tiny places across all five regions from the start (an [easter egg](easter-eggs.md)); fluent re-players spot it waiting everywhere.

## 4. Worked examples (for the team)
- Monument face (tier 2 reveal): `[IN-MEMORY-OF] tor-hal-oth-sel-eth` rendering the plain phrase "IN MEMORY OF THOSE" then a tier-3 continuation naming the uncounted.
- Tool maker's mark (Sealing Spade): a name + a date + the **THE-ENDING** logogram used as "in memory of", which is why a gardening tool quietly reads as a grave marker.
- The Gardener's word: the lone **THE-KINDNESS-TAKEN** logogram, which is the entire tragedy in one glyph.

## 5. Authoring rules
- Keep the cipher **strictly reversible** (no two plain letters share a glyph) so decode is deterministic and localizable.
- New logograms must be justified by a genuinely lost concept; do not add casual words as logograms.
- When writing inscriptions, author the **plain text** plus a `RequiredLiteracyTier`; never hand-place glyphs, let the font render them. This keeps the codex's literacy-aware reveal correct.
