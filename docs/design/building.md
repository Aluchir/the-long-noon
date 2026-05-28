# Building System Design

> Building is the cozy anchor (your home) and the progression tool (gates and outposts). It shares a placement system with crafting stations. It is also thematically loaded: in a world that has forgotten family and home, *making a place to come back to* is quietly the most radical thing the player does.

## 1. Goals
- **Cozy ownership** — a home plot the player shapes and decorates; the emotional "your spot" in the world.
- **Functional progression** — outposts (fast travel + remote crafting) and gate-builds (bridges, clearings, seals) that open regions.
- **Low-friction, forgiving** — snap-to-grid-ish, no fussy physics; build, undo, rearrange freely.
- **Thematic weight** — see §7.

## 2. What you build
- **Home base (R1 plot):** structure pieces (floors, walls, roofs), furniture, decor, crafting stations, storage.
- **Outposts (R2+):** lightweight bases at the frontier — a Wayanchor (fast-travel node) + placeable stations + storage. See [fast-travel](fast-travel-and-outposts.md).
- **Gate-builds:** region-transition constructs the Tinker designs (bridge over The Choke, clearing platforms, the Stillworks seal). These are [gating](progression-and-gating.md) requirements.
- **Comfort objects:** decor that raises the [Comfort meter](../SPRINT-1-DECISIONS.md#4-survival-mechanics-scope--tend-not-survive).

## 3. Building flow
1. **Claim** a plot (home) or place a **Wayanchor** (outpost).
2. **Place pieces** from the build menu (consume crafted components/materials).
3. **Snap & adjust** — grid/socket snapping for structures; free placement for decor.
4. **Stations & storage** placed in builds become usable crafting/storage points.
5. **Edit/remove** freely — refunds most materials to avoid punishing experimentation.

## 4. Building ↔ crafting ↔ comfort
- Build **components** are crafted (see [crafting](crafting.md)); building consumes them.
- Placed **stations** enable crafting at that location (home and outposts).
- Placed **decor** and a complete/cozy home raise **Comfort**, granting gentle buffs (faster crafting, lore insight) — reward, never requirement.

## 5. Outposts as progression
- Establishing an outpost in each region unlocks **fast travel** to it and a forward crafting/storage point, making the gather→craft loop painless near the frontier.
- Outposts are intentionally lighter than the home base — a frontier camp, not a second house.

## 6. Gate-builds as story beats
Each region transition includes a **Tinker-designed build** (bridge/clearing/seal). This:
- Teaches the building system progressively (one new build per region).
- Makes progression feel constructed by the player, not handed to them.
- Gives the Tinker recurring purpose across regions (outpost system lets them "send" designs).

## 7. Thematic layer (writers/design)
In a world with no family and no homes that mean anything (the swallowed homes of the Overgrowth are literal graves of the concept), the player **building a home and places to return to** is the game quietly reintroducing the lost idea of *belonging.* The Tinker's compulsive rebuilding and the player's deliberate home-making are two sides of the same forgotten instinct. Optional, never stated, but the system carries it.

## 8. UX
- Friendly build menu (categories: structure, station, storage, decor), ghost-preview placement, rotate/snap, undo, refund-on-remove.
- Clear feedback on gate-builds (the Tinker marks the build site; UI shows required components).
- Comfort feedback subtle and warm (no nagging meter).

## 9. Implementation notes
- Shared **placement system** for build pieces and crafting stations: `{pieceId, category, footprint, snapType, materialCost, providesStation?, comfortValue?}`.
- Plot/outpost as a claimed volume with placed-entity list; persist in [save](../tech/coding-standards-and-architecture.md).
- Gate-builds are special placeables whose completion flips a [progression](progression-and-gating.md) flag.
- Refund logic: return ~full materials on removal to encourage experimentation.
