# Fast-Travel & Outpost Design

> How the player moves between and within regions without tedium, and how frontier outposts make the gather→craft loop painless far from home. Outposts also carry a quiet theme: in a world that forgot belonging, *making places to return to* is the player reintroducing it.

## 1. Goals
- **Painless backtracking** — the world rewards revisiting earlier regions with new tools; travel must never make that a chore.
- **Frontier convenience** — craft, store, and restock near where you're working, not only at the home base.
- **Cozy, not instant-everywhere** — travel is convenient but still feels like inhabiting a place (you establish nodes, you don't teleport from anywhere to anywhere by default).

## 2. Outposts
- **Established by placing a Wayanchor** (T2 utility, craftable from R2 on) at valid frontier spots — typically one per region, plus optional extras.
- **An outpost provides:** a fast-travel node, placeable [crafting stations](crafting.md), and **linked storage** (shares with home/other outposts for craft-from-storage).
- **Lighter than home** — a frontier camp, not a second house ([building](building.md) distinguishes them).
- **Comfort/rest point** — restore Stamina/Focus, cook, and **respawn point** if the player is "downed" in [reclamation](reclamation-combat.md) (no death; you wake at the nearest outpost/home).

## 3. Fast travel
- **Node-to-node:** travel between any discovered Wayanchor/home from a node or via the map.
- **From the field → nearest node:** a "return to outpost" convenience (cozy QoL), tuned so it aids flow without trivializing exploration.
- **No mid-combat travel** during a reclamation encounter (gentle, but you finish/withdraw first).
- **Discovery-gated:** you fast-travel only to places you've established/visited — exploration still matters.

## 4. Within-region traversal
- Regions are hand-built to be **walkable and pleasant** (the cozy moment-to-moment), with traversal gear (Climb-Iron, seals) opening verticality and shortcuts as tiers rise.
- **Shortcuts unlock** as you clear Bloom / build (e.g., a cleared path or bridge becomes a permanent quick route) — reclaiming the world literally improves your mobility through it.

## 5. The linked-storage network
- All home + outpost storage is **linked** (a "stores network") so materials gathered anywhere are usable at any station — eliminates shuttling (a top cozy-game QoL).
- Optional toggle for players who prefer per-container storage (immersion preference).

## 6. Thematic layer (writers/design)
The Tinker frames the Wayanchor as *"a place you can always come back to"* — in a world where no one remembers leaving or being missed. Outposts are the mechanical expression of the game's quiet reintroduction of **belonging and return**. Never stated outright; the system carries it, and the [tone guide](../lore/tone-and-writing-style-guide.md) keeps it subtext.

## 7. UX
- **Map screen:** shows discovered nodes, current objectives, the Tinker's gate-build markers; one-click travel.
- **Outpost placement:** ghost-preview valid spots; clear feedback on what an outpost provides.
- **Stores network:** unified storage view from any station; "restock from network" one-button.
- Travel has a short, cozy transition (not a jarring cut) reinforcing place.

## 8. Implementation notes
- Wayanchor as a buildable entity that registers a `travelNode` and a `storageContainer` into the network on placement.
- Fast travel = select node → fade → spawn at node; gate by `discovered` flag.
- Respawn-on-downed resolves to nearest active node (see [reclamation](reclamation-combat.md)).
- Stores network = a storage graph queried by [crafting](crafting.md) stations (craft-from-storage) and the inventory UI.
- Shortcut unlocks flip persistent world flags saved with the region state.
