# Balance Sheet — per-tier source rates vs sink costs

> The numeric backbone the [resource economy](resource-economy.md), [crafting](crafting.md), and [tiers](weapon-and-tool-tiers.md) docs defer to ("this doc defines the structure, the sheet defines the values"). Exit costs below are derived directly from `Content/Data/Recipes.csv` and `Regions.csv`; gather rates and gate-build costs are first-pass tuning targets to refine in playtest. All values are designer knobs, not hardcoded (see [implementation notes](resource-economy.md#7-implementation-notes)).

## 1. How to read this
- **Exit cost** = the materials a player must spend to *leave* a region: the gating tool (meeting the next region's `RequiredToolTier`) plus the region's gate-build. This is the floor; optional tools/decor/food sit on top.
- **Target harvest** = exit cost x **1.4** (a deliberate ~40% surplus so the loop feels abundant, never grindy; see [anti-frustration rules](resource-economy.md#6-anti-frustration-rules)).
- **Nodes to place** = `ceil(target_harvest / (yield_per_node x 0.7))`, where 0.7 assumes a normal session engages ~70% of a region's nodes. Signature material yields **3/node**, secondary **2/node**.
- Components expand to their own inputs: `comp_binding` = softfiber 1 + resin 1; `comp_fitting` = stillsteel 1 + sealstone 1.

## 2. Gating path per transition (from `Regions.csv`)
| Transition | Req tool tier | Gating tool (recipe) | Traversal | Gate-build |
|---|---|---|---|---|
| R1 → R2 | 1 | Pruning Blade (`recipe_pruning_blade`) | clear treeline | `gate_treeline_thicket` |
| R2 → R3 | 2 | Stiller (`recipe_stiller`) | The Choke | `gate_the_choke` |
| R3 → R4 | 3 + Seal | Sealing Spade (`recipe_sealing_spade`) | sealed passage | `gate_sealed_passage` |
| R4 → R5 | 4 | Long Tool (`recipe_long_tool`) | the Threshold | `gate_threshold` |

## 3. Gate-build costs (design target — not yet in data)
Gate-builds are referenced by `Regions.RequiredGateBuild` but have no recipe rows yet. These are the intended costs; author them into a future `Builds.csv` (one row per gate, same `inputs` encoding as `Recipes.csv`). Each pulls mostly the *current* region's materials, with a small **preview** of the next tier to pull the player forward (per [resource economy §1](resource-economy.md#1-principles)).

| Gate-build | Inputs (current tier) | Preview (next tier) |
|---|---|---|
| `gate_treeline_thicket` | gentlestone 4; softfiber 4; sunmoss 6 | — |
| `gate_the_choke` | swallowedwood 6; resin 4; glowcap 2; `comp_binding` 2 | stillsteel 1 |
| `gate_sealed_passage` | sealstone 6; stillsteel 4; `comp_fitting` 2 | hushglass 1 |
| `gate_threshold` | hushglass 4; stillsteel 3; `comp_fitting` 2 | — |

## 4. Exit cost ledger (gating tool + gate-build, components expanded)

### R1 The Sunhollow → R2 (tier-1 materials)
| Material | Pruning Blade | gate_treeline_thicket | **Exit total** | Target harvest (x1.4) | Yield/node | Nodes |
|---|---|---|---|---|---|---|
| gentlestone | 2 | 4 | **6** | 8.4 | 2 | 6 |
| softfiber | 1 | 4 | **5** | 7.0 | 2 | 5 |
| sunmoss | 0 | 6 | **6** | 8.4 | 3 | 4 |

### R2 The Overgrowth → R3 (tier-2 materials; + tier-3 preview)
| Material | Stiller (+binding) | gate_the_choke (+binding) | **Exit total** | Target harvest (x1.4) | Yield/node | Nodes |
|---|---|---|---|---|---|---|
| swallowedwood | 2 | 6 | **8** | 11.2 | 3 | 6 |
| resin | 3 | 6 | **9** | 12.6 | 2 | 9 |
| glowcap | 0 | 2 | **2** | 2.8 | 2 | 2 |
| softfiber (carry) | 1 | 2 | **3** | — | — | from R1 |
| stillsteel (preview) | 0 | 1 | **1** | — | — | R3 edge node |

### R3 The Stillworks → R4 (tier-3 materials; + tier-4 preview)
| Material | Sealing Spade (+fitting) | gate_sealed_passage (+fitting) | **Exit total** | Target harvest (x1.4) | Yield/node | Nodes |
|---|---|---|---|---|---|---|
| stillsteel | 3 | 6 | **9** | 12.6 | 3 | 6 |
| sealstone | 1 | 8 | **9** | 12.6 | 2 | 9 |
| swallowedwood (carry) | 1 | 0 | **1** | — | — | from R2 |
| hushglass (preview) | 0 | 1 | **1** | — | — | R4 edge node |

### R4 The Hush → R5 (tier-4 materials)
| Material | Long Tool (+fitting) | gate_threshold (+fitting) | **Exit total** | Target harvest (x1.4) | Yield/node | Nodes |
|---|---|---|---|---|---|---|
| hushglass | 2 | 4 | **6** | 8.4 | 3 | 4 |
| stillsteel (carry) | 2 | 5 | **7** | — | — | from R3 |
| sealstone (carry) | 1 | 2 | **3** | — | — | from R3 |

> R5 The First Garden has no gather loop and no tool (`Bloom density: None`). `mat_gardenlight` is narrative-only (given or withheld by the ending), `MaxStack 1`, `bDroppable false`.

## 5. Durability & repair rhythm
- Wear is **`DurabilityCostPerUse = 1`** per successful reclamation (`ULongNoonReclamationComponent`). Tool `MaxDurability` from `Tools.csv` runs 100–200, so a tool lasts **100–200 reclamations** between repairs — a gentle rhythm, not busywork ([resource economy §5](resource-economy.md#5-balance-approach)).
- A worn tool (durability 0) cannot reclaim until repaired; it is a setback, never destruction ([reclamation-combat](reclamation-combat.md)). `Repair(Amount)` restores durability, clamped to max.
- **Repair kit (design target — not yet in data):** add `item_repair_kit` to `Items.csv` and a `recipe_repair_kit` costing **current-tier signature x2** (e.g. R1: sunmoss 2). Restores ~50% of a tool's `MaxDurability`. Cheap and craftable from abundant mats so upkeep stays a rhythm.

## 6. Stamina sink check (vs `ULongNoonTendComponent`)
`MaxStamina 100`, `StaminaRegenPerSecond 12` (full regen ~8s). Per-use costs:
- **Crafting** (`Recipes.StaminaCost`): 0 for tier-1, 5 for tier-2 tools, 10–12 for tier-3/4. A tier-3 craft is ~10% of the bar; never a wall.
- **Reclamation** (`Tools.StaminaCost`): 5 (Pruning Blade) → 16 (Long Tool). At regen 12/s, the player can sustain roughly one reclaim every 1–1.5s indefinitely; bursts deplete and slow, never fail.

## 7. Open data tasks this sheet implies
- [ ] `Builds.csv` (+ `FBuildRow` / building system) for the four gate-builds in §3.
- [ ] `item_repair_kit` + `recipe_repair_kit` in `Items.csv` / `Recipes.csv` (§5).
- [ ] Node counts in §4 are placement targets for the greybox/level pass, not yet authored as actors.

## 8. Source of truth
Exit costs and stamina/durability figures here are computed from the committed CSVs — re-derive them whenever `Recipes.csv`, `Tools.csv`, or `Regions.csv` change. Gather rates and gate/repair costs are tuning targets; move them into data (§7) and retune against playtest, never into code.
