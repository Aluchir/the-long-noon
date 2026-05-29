# Art Asset List

> The production manifest for visual assets, derived from the [style guide](style-guide.md). Organized so a producer can track it and an artist can pull a clear next task. Scope discipline is the law: **low-poly stylized, modular region kits, baked lighting** ([style guide §9](style-guide.md#9-production-notes--scope)). Build greybox → Region 1 vertical slice → art pass. Folder roots map to `Content/` (see [first-playable checklist](../tech/first-playable-checklist.md)).

## Conventions
- **Priority:** P0 = first-playable vertical slice (Sunhollow); P1 = full critical path; P2 = polish/cozy layer.
- **No baked-in text on any asset** (inscriptions are font-rendered so they localize for free — see [localization](../tech/localization.md)).
- Per-region look follows the **two dials** (light angle + saturation/warmth) from [style guide §4–5](style-guide.md#4-color--light).

## 1. Characters (`Content/Characters/`)
| Asset | Notes | Priority |
|---|---|---|
| The Newcomer (player) | rounded cozy proportions; **tell: a longer shadow**; reparent to `ALongNoonCharacter` | P0 |
| The Tinker | cheerful builder; Sunhollow | P0 |
| The Keeper | **tell: funeral-formal dress**; Sunhollow/Stillworks | P1 |
| The Forager | Overgrowth | P1 |
| The One Who Counts | Stillworks; stylus prop | P1 |
| The Rememberer | The Hush (hidden) | P2 |
| The Gardener | First Garden; distinct, uncanny silhouette; held glassy presence | P2 |
| Fading Ones (ambient) | half-present background figures (Hush) | P2 |
| Shared rig + 1 "tell" per deep character | one expressive rig; cozy idle/talk/gather/craft/prune anims | P0 |

## 2. Bloom & reclamation VFX (`Content/Bloom/`, `Content/VFX/`)
| Asset | Notes | Priority |
|---|---|---|
| Bloom meshes ×5 tiers | friendly/abundant (R1) → architectural/strange (deep); never gross | P0 (R1), P1 (rest) |
| Gather-node visual states | active / harvested / regrowing (`AGatherNode`) | P0 |
| Bloom target states | Active / Dormant / Settled (`ABloomActor`) | P0 |
| **Still** VFX | calming light-pulse + slow bloom-close | P0 |
| **Settle** VFX | soft motes drifting to sleep | P1 |
| Prune/seal/quiet VFX | gentle, non-violent per verb | P1 |
| Ambient pollen/light drift; Bloom "breathing" | per-region density | P1 |

## 3. Region environment kits (`Content/Environments/<region>/`)
Modular kit per region (reusable pieces, [style guide §6](style-guide.md#6-environment--props)). Each kit: ground/terrain pieces, foliage set, rocks/props, structure pieces, region-specific set dressing.
| Kit | Mood / palette target | Priority |
|---|---|---|
| R1 Sunhollow | brightest golden hour, warmest | **P0** |
| R2 Overgrowth | amber, lush deep greens, smothering | P1 |
| R3 Stillworks | bronze, first desaturation, tidy emptiness | P1 |
| R4 The Hush | near-horizontal pale light, uncanny stillness | P2 |
| R5 First Garden | gold restored, radiant, frozen-perfect | P2 |

## 4. Hero set-pieces (authored with extra care)
| Asset | Region | Notes | Priority |
|---|---|---|---|
| The Monument | R1/R3 | Keeper's; inscription = literacy payoff | P0 |
| The Hall of Marks | R3 | One Who Counts' tally wall; eroded late marks | P1 |
| Dormant Stillworks machines | R3 | pointedly clinical (the one non-rounded form) | P1 |
| Swallowed homes | R2 | Bloom-eaten structures; excavation lore | P1 |
| The First Garden core | R5 | the source; the held-breath stillness | P2 |
| Gate-builds ×4 | per [balance sheet §3](../design/balance-sheet.md#3-gate-build-costs-design-target--not-yet-in-data) | treeline / Choke / sealed passage / Threshold | P1 |

## 5. Tools, items & stations (`Content/Models/Items/`)
| Set | Source | Priority |
|---|---|---|
| Reclamation tools ×6 + utility ×3 | `Tools.csv` (Pruning Blade … Long Tool, Field Hook, Wayanchor, Climb-Iron) | P0 (T1), P1 (rest) |
| Materials/components/consumable icons + meshes | `Items.csv` (13 entries) | P0 (T1), P1 (rest) |
| Crafting stations ×3 | Basic Bench (R1), Forager's Bench (R2), Stillworks Forge (R3) | P0 (bench), P1 |
| Cozy decor + building component kit | the [reward layer](style-guide.md#6-environment--props); home/outposts | P2 |

## 6. UI art (`Content/UI/`)
| Asset | Notes | Priority |
|---|---|---|
| HUD kit | Tend meters, interact prompt, lore toast (`ULongNoonHUDWidget` events); colorblind-safe pastel | P0 |
| Crafting UI | inputs/output preview, missing-material highlight, craft-from-storage | P0 |
| Codex UI | glyph vs translated text slots; literacy-aware reveal | P1 |
| Dialogue UI | line box + speaker; wordless-vocalization paired | P1 |
| **Forgotten-script cipher font** | per [font spec](forgotten-script-font-spec.md); VF or 3 erosion statics | P1 |
| Icon set | tools/items/verbs; readable at a glance | P1 |
| Soft rounded panels / cozy click language (visual) | shared style sheet | P1 |

## 7. Lighting & look-dev
| Asset | Notes | Priority |
|---|---|---|
| Master palette + light-dial reference sheet | the measurable region steps; screenshot-comparable | P0 |
| Per-region baked lighting setups ×5 | no runtime sun cycle | P0 (R1), P1/P2 |
| Shared style sheet (proportions, edge-softness, texturing) | multi-artist coherence | P0 |

## 8. Open dependencies
- Region kits before bespoke props; greybox before art pass.
- Cipher font (P1) blocks readable inscriptions but **not** the greybox (placeholder font OK).
- Gate-build meshes pair with the `Builds.csv` data task ([balance sheet §7](../design/balance-sheet.md#7-open-data-tasks-this-sheet-implies)).
