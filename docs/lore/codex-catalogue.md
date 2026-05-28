# Codex / Lore-Fragment Catalogue

> The codex is the journal where the player's rediscovered truth accumulates. Fragments are the world's memory, externalized — the player collects them, and as literacy grows, they resolve into meaning. This catalogue lists the fragment system and a seed set per region.

## 1. The codex system
- **Purpose:** the only place the world's forgotten history lives. The player is the world's returning memory; the codex is its storage.
- **Fragment types:**
  - **Inscriptions** — fixed text in the world (monuments, walls, machines). Readability scales with [script literacy](forgotten-script.md).
  - **Artifacts** — recovered objects with flavor + a lore note (a kept tool, a census ledger, a child's toy that shouldn't exist).
  - **Observations** — the Newcomer's own journal reactions to events (the Keeper's tears, a shadow falling wrong).
  - **Testimonies** — fragments of NPC speech the player chooses to record.
  - **True-names** — the Forager-thread botanical vocabulary.
- **Living readability:** entries logged at low literacy show as glyphs; revisiting after a literacy gain reveals the translation. The codex visibly *fills in* as the player learns the script.
- **Thread view:** fragments are tagged to **threads** (Monument, Tally, Machines, True-Names, The Gardener, The Rememberer) so the player can see a mystery assembling.
- **No required reading.** Critical path is understandable without 100% collection; completion rewards the thorough with the full, devastating picture.

## 2. The major threads
| Thread | Anchored by | Resolves into |
|---|---|---|
| **Monument** | the Keeper (R1, R3) | the world's forgotten practice of mourning the pre-Noon dead |
| **Tally** | the One Who Counts (R3) | the count = the pre-Noon population; the dead were real and numberless |
| **Machines** | the Stillworks (R3) | infrastructure built for *ending,* now purposeless |
| **True-Names** | the Forager (R2) | a lost language being unknowingly grieved and reinvented |
| **The Gardener** | distant rumor → First Garden (R5) | the kind catastrophe; the act of mercy that took everything |
| **The Rememberer** | hints R1+ → the Hush (R4) | the one unforgetting mind; the key to the Third Way |

## 3. Seed fragments per region
> A starting set, not exhaustive — content team expands. Format: **[type]** title — note.

### Region 1 — The Sunhollow
- **[Inscription]** *The Monument's Face* — unreadable now; the keystone payoff (Monument thread).
- **[Observation]** *A Shadow Falls Wrong* — the Newcomer notes their own shadow differs (player mortality seed).
- **[Testimony]** *No One Asks* — the player records that nobody asked where they came from.
- **[Artifact]** *The Tinker's Kept Tool* — one object older than memory the Tinker can't scrap (pre-Noon remnant).

### Region 2 — The Overgrowth
- **[True-name]** *Morningspit (true name recovered)* — first script word matched by context (True-Names thread; literacy tier 1).
- **[Artifact]** *A Swallowed Home* — evidence these ruins were dwellings that held more than one kind of bond (family seed).
- **[Inscription]** *Bloom-warden's Note* — confirms the Bloom is post-Event runaway growth.

### Region 3 — The Stillworks
- **[Inscription]** *Memorial Dedication* — first readable sentence (script-key / literacy tier 2); ties monument + the word for the lost thing.
- **[Artifact]** *Census Ledger Fragment* — the number that, with the Hall of Marks, reveals the tally = pre-Noon population.
- **[Inscription]** *Machine Plate* — schematic label describing a function tied to ending/decay.
- **[Observation]** *Someone Remembers* — a hint that one mind never eroded (Rememberer seed).

### Region 4 — The Hush
- **[Inscription]** *The Last Fragments* — complete the grammar (literacy tier 3).
- **[Artifact]** *The Rememberer's Archive* — optional, the fullest intact history in the game.
- **[Testimony]** *A Fading One Loops* — a near-erased mind repeating half a sentence forever.
- **[Inscription]** *Confirmation* — readable account of the Gardener's act (climax setup).

### Region 5 — The First Garden
- **[Inscription]** *The Gardener's Word* — the single glyph for *"the kindness I took away"* (literacy tier 4 / fluent).
- **[Testimony]** *Is Everyone Happy Now?* — the Gardener's question, recorded.
- **[Observation]** *The Light's Source* — the Newcomer realizes the Sunhollow's warmth always came from here.

## 4. Implementation notes
- Each fragment: `{id, type, thread, region, glyphs?, translation?, requiredTier, flavor}`.
- Codex UI groups by **thread** and by **region**; shows a completion meter per thread (spoiler-safe titles until found).
- Literacy gains trigger a **re-scan** that upgrades readable fragments and can fire a "you can now read…" prompt for the most significant ones (the Sunhollow monument is the marquee retroactive reveal).
