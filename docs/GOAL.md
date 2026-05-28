# Build-Out Goal — "everything achievable headlessly"

> Driven by an autonomous /loop. The loop completes this backlog, then stops.

## The goal
Take *The Long Noon* from "scaffold + stubs" to the **maximum state achievable without a running Unreal Engine install** — all code systems drafted into coherent, consistent, review-clean C++; all story/text/data authored; and a precise in-editor checklist that takes a human from this repo to the first playable run.

## The honest ceiling (what the loop CANNOT do)
These require Unreal Engine 5.4 installed + the GUI editor, so they are **documented for the user, not done by the loop**:
- Compile the C++ (needs UnrealBuildTool + engine headers).
- Generate/author binary content (`.uasset`/`.umap`): the Third Person template character, Enhanced Input assets, materials, the Sunhollow map.
- Launch/playtest the game.

The loop's "done" = everything below is authored and committed, and `docs/tech/first-playable-checklist.md` precisely lists the remaining in-editor steps.

---

## Backlog

### A. Code completeness (C++, `Source/TheLongNoon/`)
- [x] Project log category (`LogLongNoon`)
- [x] `IInteractable` interface + character interact trace implementation
- [x] `ABloomActor` / reclamation target (dormant/settled states, required verb, drop table)
- [x] `AGatherNode` actor (harvestable, regrows)
- [x] `ULongNoonCraftingComponent` (consume recipe inputs, produce output, station + stamina, broadcast)
- [x] `ULongNoonBuildingComponent` (placement, gate-build completion hooks)
- [x] `ULongNoonTendComponent` (Stamina/Focus/Comfort meters; no death)
- [x] `ULongNoonDialogueComponent` (literacy-aware lines, codex hooks)
- [x] `ULongNoonQuestSubsystem` (objective tracking)
- [x] Reclamation: equip tool from inventory by tier; resolve target verb
- [x] HUD base (`ALongNoonHUD` + UMG base widget); add `UMG` to Build.cs
- [x] Wire `SaveService` TODOs (gather/restore inventory + found fragments)
- [x] Wire components onto the character / register subsystems

### B. Story & text (`docs/lore/`, `docs/design/`)
- [x] Dialogue scripts per character (Tinker, Forager, Keeper, One Who Counts, Gardener, Rememberer)
- [x] Full codex fragment text per region (expand the catalogue into actual fragments)
- [x] Forgotten-script cipher mapping (a real substitution table)
- [x] The three ending scripts (Gift Returned / Long Noon Continues / Third Way)
- [x] Per-region quest/beat flow (objectives, gates, lore beats)
- [x] Weapon/tool + material flavor finalized as data-ready entries (docs/lore/weapon-and-tool-lore.md)

### C. Data (authoring-ready, importable as UE DataTables — CSV is text)
- [x] `Content/Data/Items.csv`
- [x] `Content/Data/Tools.csv`
- [x] `Content/Data/Recipes.csv`
- [x] `Content/Data/Regions.csv`
- [x] `Content/Data/LoreFragments.csv`
- [x] Data authoring guide (how the CSVs map to the DataAsset/DataTable structs)

### D. Run-enablement (the bridge to a human-run build)
- [x] `docs/tech/first-playable-checklist.md` — exact ordered UE editor steps to first run
- [x] `docs/tech/build-and-run.md` — headless build commands (for when UE is installed) + CI notes
- [x] Final consistency pass (code references, doc links, Ithura sync of new work)

---
### Done so far (prior turns)
World Bible, Sprint-1 decisions, full character bible, all lore + design specs, art/audio direction, UE5 scaffold + C++ system stubs, Ithura board (Unreal-consistent).
