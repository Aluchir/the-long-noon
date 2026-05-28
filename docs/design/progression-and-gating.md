# Progression & Region-Gating Design

> How the player advances, and how gear/crafting tiers unlock regions. The spine that makes "cozy crafting loop" and "narrative descent into truth" the *same* progression. Geography = chronology = revelation.

## 1. The core principle
There is **one progression axis** and it does triple duty:
- **Mechanical:** better tools/gear → handle denser Bloom → reach the next region.
- **Spatial:** each region is physically deeper/older than the last.
- **Narrative:** each region reveals a deeper layer of the buried truth.

Advancing your gear *is* advancing the story. The player never grinds for grinding's sake — every tier is a key to the next chapter.

## 2. The tier ↔ region matrix
| Tier | Region | Bloom density | Key new capability | Story layer unlocked |
|---|---|---|---|---|
| 1 | The Sunhollow | Gentle | Pruning Blade; basic build | The monument exists; warmth hides something |
| 2 | The Overgrowth | Dense, animate | The Stiller; Resonance Hammer; outposts | Bloom is unnatural; swallowed homes; true-names |
| 3 | The Stillworks | Sparse, managed | Sealing Spade; climb/seal; script-key | The tally; machines of ending; readable script |
| 4 | The Hush | Slow, sleeping | The Long Tool; advanced traversal | Memory's end; the Rememberer; full literacy |
| 5 | The First Garden | None | (no tool — nothing to cut) | The Gardener; the truth; the choice |

## 3. What gates a region transition
Each gate is a **small composite** (never a single fetch), so it feels earned and teaches the next region's pressure:
1. **A tool tier** — crafted from the *previous* region's materials.
2. **A traversal/clear capability** — the physical obstacle (a thicket, The Choke, a sealed passage, the Threshold).
3. **A Tinker build** — a constructed object (bridge, clearing, seal) that doubles as a building-system beat.

> Example (R1 → R2): craft the Pruning Blade (tier 1) + clear the treeline thicket + complete the Tinker's opening build → The Overgrowth opens.

## 4. Progression currencies (all diegetic, no abstract XP)
- **Materials** (tiered, region-sourced) — the primary currency; feeds crafting. See [resource economy](resource-economy.md).
- **Recipes / schematics** — unlocked via the Tinker and via recovered fragments.
- **Script literacy** — a parallel progression earned by collecting [lore fragments](../lore/codex-catalogue.md); gates *understanding,* not movement (mostly), though tier 3 needs the script-key.
- **Comfort** (home/decor) — soft, optional; grants gentle buffs, never required.

No kill-XP, no levels. You progress by *making and learning,* matching the cozy fantasy.

## 5. Pacing & gating philosophy
- **Soft gates over hard walls** where possible — the player should usually *see* where they can't yet go and understand why (a too-dense Bloom, a too-high climb), creating pull rather than confusion.
- **Backtracking is rewarded, not punished** — new tools open earlier regions' optional Bloom/lore (fast travel via outposts makes this painless; see [fast-travel](fast-travel-and-outposts.md)).
- **Difficulty curve is gentle and player-tunable** — reclamation intensity is a setting (see [reclamation](reclamation-combat.md)); the *real* curve is material/tool depth, not twitch challenge.
- **Story beats are gated to regions, not to optional content** — a player who ignores deep lore still gets the critical-path understanding; thorough players get the full picture and the True Ending.

## 6. The optional depth layer (True Ending path)
Parallel to the main gating, a **deep-lore track** gates the best ending:
- Collect enough [fragments](../lore/codex-catalogue.md) → reach script literacy tiers → find the **Rememberer** in R4 → unlock the **Third Way** ending in R5.
- Entirely optional; never blocks finishing the game. Rewards attention with the most hopeful resolution.

## 7. Implementation notes
- Model progression as: `region.unlocked = (toolTier >= req) && (traversalCap.has(x)) && (build.completed(y))`.
- Keep gate requirements **data-driven** (per-region requirement objects) so designers can retune without code.
- Track `scriptLiteracyTier` and `rememberOathFound` flags separately from movement gates.
- Surface gate requirements clearly in the UI (the Tinker tells you what you still need) — no guesswork.
