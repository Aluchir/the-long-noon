# Crafting System Design

> Crafting is the heart of the loop: gather → **craft** → build → reclaim → progress. It is cozy, legible, and tiered to gate regions. It is also a lore-delivery surface (every crafted item carries [flavor](../lore/weapon-and-tool-lore.md)).

## 1. Goals
- **Satisfying and clear**, never fiddly. Cozy-game craftability: obvious inputs, visible outputs, no obscure combinatorics.
- **Tiered** so it gates regions (tier N tools need tier N materials from region N).
- **Diegetic** — taught and gated by the Tinker, not menus alone.
- **Lore-bearing** — recipes and items reveal world truth.

## 2. What you craft
- **Reclamation tools** (Pruning Blade → Stiller → Sealing Spade → Long Tool) — the gating items. See [tiers](weapon-and-tool-tiers.md).
- **Utility tools** (Field Hook, Wayanchor/outpost markers, Climb-Iron, Builder's Kit).
- **Building components** — feed the [building system](building.md).
- **Consumables** — food (restores Stamina/Focus, raises Comfort), tool-repair kits.
- **Decor** — cosmetic, raises Comfort; the cozy reward layer.

## 3. Crafting flow
1. **Gather** materials from the Bloom / ruins (see [resource economy](resource-economy.md)).
2. **Unlock a recipe** — via the Tinker (taught) or a recovered schematic (found).
3. **Craft at a station** — workbench tiers; later regions may need region-specific stations (e.g., a Stillworks forge for Stillsteel).
4. **Spend Stamina/Focus** on heavy crafts (ties to the [Tend meters](../SPRINT-1-DECISIONS.md#4-survival-mechanics-scope--tend-not-survive)); never a fail-state, just pacing.
5. **Upgrade** existing tools to the next tier rather than always rebuilding (keeps attachment to "your" tools).

## 4. Stations (tiered)
| Station | Region | Enables |
|---|---|---|
| Basic Workbench | R1 | tier 1 tools, basic components, food |
| Forager's Bench | R2 | tier 2 tools, the Stiller, field-guide upgrades |
| Stillworks Forge | R3 | tier 3 tools (Stillsteel), seals, climb gear |
| (uses Forge) | R4 | tier 4 Long Tool from Hushglass |

Stations are **buildable** (overlap with [building](building.md)) and **placeable at outposts**, so the player can craft near the frontier.

## 5. Recipes & discovery
- **Taught recipes:** the Tinker unlocks the critical-path tools and a baseline of utility/decor.
- **Found recipes:** schematics recovered from ruins (especially Stillworks machine schematics) unlock optional/advanced items and double as [lore fragments](../lore/codex-catalogue.md).
- **No random drops for critical items** — the player can always deterministically craft what they need to progress.

## 6. Economy & balance
- Each tier's tools cost that tier's materials → enforces the gather-before-advance loop.
- **Upgrade paths** are cheaper than full rebuilds, rewarding investment in a tool.
- Decor/comfort items use abundant low-tier materials so the cozy layer stays generous.
- Repair kits keep tool **durability** (the main upkeep pressure) manageable without busywork.
- Full numeric tuning lives in a balance sheet alongside the [resource economy](resource-economy.md).

## 7. UX
- Clean, friendly crafting UI (see UI module): inputs, output preview, "missing material" highlighting, one-click craft + craft-from-storage.
- **The Tinker tells you what's gating progress** ("you'll want a sturdier blade for that Choke") — guidance, not guesswork.
- Crafted-item tooltips show [flavor text](../lore/weapon-and-tool-lore.md) (lore-bearing, literacy-aware).

## 8. Implementation notes
- Data-driven recipes: `{id, station, tier, inputs:[{mat,qty}], output, unlockSource, staminaCost}`.
- Tool upgrade as a recipe variant: `{from:toolTierN, to:toolTierN+1, inputs}`.
- Tie tool tier into the [gating](progression-and-gating.md) check (`toolTier >= region.req`).
- Keep stations as buildable entities so crafting and building share one placement system.
