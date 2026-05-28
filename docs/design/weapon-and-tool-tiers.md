# Weapon & Tool Tier Design

> The mechanical spec for the reclamation tools and utility gear. Tiers map 1:1 to regions and Bloom densities, so tool progression *is* region progression. Pairs with the [lore flavor](../lore/weapon-and-tool-lore.md) and the [progression matrix](progression-and-gating.md).

## 1. Tier philosophy
- **Five tiers, five regions.** Each region's Bloom requires the matching tool tier; you craft tier N from region N's materials to enter region N+1.
- **Upgrade, don't replace.** Tools upgrade along their line (you keep "your" blade), reinforcing attachment and the cozy fantasy.
- **Each tier adds a verb or a capability,** not just bigger numbers — progression feels like new ability, not stat inflation.
- **Tier 5 is the absence of a tool** — the First Garden has nothing to cut (thematic, see [region 5](../lore/region-5-the-first-garden.md)).

## 2. Reclamation tool lines
| Tier | Tool | Verb added | Material | Handles | Region |
|---|---|---|---|---|---|
| 1 | Pruning Blade | **Prune** | Sunmoss + basic | gentle inert Bloom | Sunhollow |
| 2 | The Stiller | **Still** (dormant) | Swallowed-wood | dense Bloom; safe passage/harvest | Overgrowth |
| 2 | Resonance Hammer | **Settle** | Swallowed-wood + stone | animate growths (non-lethal) | Overgrowth |
| 3 | Sealing Spade | **Seal** + climb | Stillsteel | managed Bloom; constructs; traversal | Stillworks |
| 3 | Quieting Edge | refined Settle | Stillsteel | tougher constructs | Stillworks |
| 4 | The Long Tool | **Quiet** (all-in-one) | Hushglass | the slow Bloom of the Hush | The Hush |
| 5 | — | (none) | — | nothing to subdue | First Garden |

## 3. Tool stats (axes, not just damage)
Each tool has gentle, readable stats — tuned in a balance sheet, illustrative here:
- **Reach / area** — how much Bloom a swing affects.
- **Still duration** (Stiller/Long Tool) — how long a patch stays dormant.
- **Settle power** — how quickly an animate growth/construct is sent to sleep.
- **Stamina/Focus cost** per use (ties to [Tend meters](../SPRINT-1-DECISIONS.md#4-survival-mechanics-scope--tend-not-survive)).
- **Durability** — uses before repair (the main upkeep pressure; see [inventory/economy](inventory-and-economy.md)).
- **Yield bonus** — better tools harvest more/cleaner materials.

> No "damage" in the lethal sense. "Settle power" replaces it; nothing has a health-to-death bar, only a settle-threshold.

## 4. Upgrade paths
- Each tool has 1–2 **upgrade steps within its tier** (e.g., Pruning Blade → Honed Pruning Blade) for finer pacing, then a **tier jump** at the next region.
- Upgrades cost mixed materials (mostly current tier + a little next-tier preview material) to pull the player forward.
- Upgrading preserves the tool identity and flavor; the [lore text](../lore/weapon-and-tool-lore.md) deepens with tier.

## 5. Utility gear tiers
| Gear | Tier/Region | Function |
|---|---|---|
| Field Hook | T1 / R1 | gathering yield/speed |
| Builder's Kit | T1 / R1 | [building](building.md) placement |
| Wayanchor | T2 / R2 | establish outpost + [fast travel](fast-travel-and-outposts.md) |
| Climb-Iron | T3 / R3 | vertical traversal |
| Bag/Satchel upgrades | all | inventory capacity |

## 6. Gating hooks
- A region's entry check reads `maxReclamationToolTier >= region.requiredTier` plus traversal capability + gate-build (see [progression](progression-and-gating.md)).
- Verbs unlocked by tools are checked by [reclamation targets](reclamation-combat.md) (`target.requiredVerb in player.verbs`).

## 7. Implementation notes
- Tool data: `{id, line, tier, verbs:[], stats:{reach, stillDur, settlePower, staminaCost, durability, yield}, upgradeFrom?, material, flavorRef}`.
- Verbs are first-class so [reclamation](reclamation-combat.md) and [gating](progression-and-gating.md) both query them.
- Durability + repair shared with [inventory/economy](inventory-and-economy.md).
- Tier→region requirement table is the single source for [gating](progression-and-gating.md).
