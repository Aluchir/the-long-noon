# Resource Economy (per region)

> The material layer that feeds the whole loop. Materials are tiered and region-sourced, so gathering a region affords that region's tools, which open the next region. This is the numeric backbone of [progression](progression-and-gating.md), [crafting](crafting.md), and [inventory/economy](inventory-and-economy.md).

## 1. Principles
- **One tier of materials per region.** Region N yields tier-N materials; tier-N tools/builds require them. Clean, legible, gating.
- **A preview material** from region N+1 is needed for the *last* upgrade in region N, pulling the player forward.
- **Closed loop, no money.** Materials are the only economy (see [inventory/economy](inventory-and-economy.md)).
- **Abundance for cozy, scarcity for progression.** Decor/comfort materials are plentiful; tool/gate materials are paced.
- **Materials carry lore.** Each signature material has [flavor](../lore/weapon-and-tool-lore.md) tying it to its region's truth.

## 2. Material table by region
| Region | Signature material | Secondary | Source | Used for |
|---|---|---|---|---|
| R1 Sunhollow | **Sunmoss** | soft fibers, gentle stone | meadow Bloom, easy nodes | T1 tools, basic builds, decor, food |
| R2 Overgrowth | **Swallowed-wood** | resin, glowcaps (fungal) | ruin excavation, dense Bloom | T2 tools (Stiller/Hammer), outposts |
| R3 Stillworks | **Stillsteel** | machine salvage, seal-stone | dormant machines, managed Bloom | T3 tools, climb/seal gear, forge builds |
| R4 The Hush | **Hushglass** | thin-light crystal | slow Bloom, the Hush's quiet nodes | T4 Long Tool, late builds |
| R5 First Garden | **Gardenlight** | — | not gathered; given/withheld by ending | narrative only |

## 3. Sources (how materials enter)
- **Bloom harvesting / pruning** — primary, renewable; regrows over "time" (Bloom regrowth loop).
- **Ruin excavation** — clearing Bloom off swallowed structures (R2) / machines (R3); yields materials *and* [lore fragments](../lore/codex-catalogue.md).
- **Reclamation byproducts** — settling constructs drops salvage (R3+).
- **Region nodes** — fixed gather points (gentle stone, fungal groves, Hush crystal).
- **Renewability:** every critical material is renewably gatherable; no permanent scarcity that could soft-lock progression.

## 4. Sinks (how materials leave)
- **Tool crafting & upgrades** (the main progression sink) — see [tiers](weapon-and-tool-tiers.md).
- **Building** — home, outposts, and **gate-builds** (the bridge/clearing/seal that open regions).
- **Consumables** — food (Stamina/Focus, Comfort), repair kits (durability).
- **Decor** — cosmetic Comfort sink; uses abundant low-tier mats.

## 5. Balance approach
- **Per-tier ledger:** for each region, sum the materials needed to craft the tools + gate-build to *leave* it, and tune that region's source rates so a normal play session through the region covers it with a comfortable surplus (cozy, not grindy).
- **Surplus funds the cozy layer** (decor, comfort food) — intentional, so players feel abundant, not pinched.
- **Upgrade pacing:** within-tier upgrades are cheap; the tier jump (with its preview material) is the meaningful cost.
- **Repair economy:** durability tuned so repair is a gentle rhythm, repair kits cheap from current-tier mats.
- Numbers live in a shared **balance sheet** with [crafting](crafting.md) and [tiers](weapon-and-tool-tiers.md); this doc defines the structure, the sheet defines the values.

## 6. Anti-frustration rules
- **No grind walls:** never require more of a material than a region's normal traversal yields plus a small margin.
- **No soft-locks:** all progression materials are renewable and reachable with current-tier tools.
- **Overflow is safe** (see [inventory](inventory-and-economy.md)) — gathered material is never lost.
- **Craft-from-storage + outpost storage** keep the loop low-friction (see [fast travel](fast-travel-and-outposts.md)).

## 7. Implementation notes
- Materials: `{id, tier, region, renewable:true, sources:[], flavorRef}`.
- Region "exit cost" computed from tool + gate-build recipes; gather rates tuned against it per region.
- Bloom regrowth timers drive renewability; node respawns on the same activity clock.
- Expose tuning knobs (source rate, sink cost, durability) in the balance sheet, not code.
