# First Playable Checklist (the human-at-the-editor steps)

> This is the terminal deliverable of the headless build-out. Everything achievable without Unreal is done and committed on `feature/unreal-scaffold`. The steps below **require Unreal Engine 5.4 installed and the GUI editor**, because they produce binary content (`.uasset`/`.umap`) and need compilation and Play-in-Editor. Do these in order to reach the first playable Sunhollow slice.

> Honest status: the C++ here is written to UE5 conventions but has **not been compiled** (no engine on the authoring host). Expect to fix a few includes on first build; that is normal for hand-authored UE C++.

## 0. Prerequisites
- [ ] Install **Unreal Engine 5.4** (Epic Games Launcher) and a C++ toolchain (Visual Studio 2022 on Windows, Xcode on macOS).
- [ ] Install **Git LFS** and run `git lfs install` (mandatory before committing any `Content/`).
- [ ] Clone the repo and check out `feature/unreal-scaffold`.

## 1. Generate the playable base from the Third Person template
The character/camera/input/sample assets are binary and come from Epic's template.
- [ ] In the Launcher: New Project -> Games -> **Third Person**, **C++**, Desktop, name it `TheLongNoon` in a scratch location.
- [ ] Copy the generated `Content/ThirdPerson/...` character, the Enhanced Input assets (`IMC_Default`, `IA_*`), and the sample map into this repo's `Content/` (Characters/Player, Input, Maps).
- [ ] Keep this repo's `Source/`, `Config/`, and `.uproject` (module name `TheLongNoon` already matches the template's expectations).

## 2. Compile the C++ module
- [ ] Right-click `TheLongNoon.uproject` -> Generate project files.
- [ ] Open in the IDE and build (or open the `.uproject` and accept the "rebuild missing modules" prompt).
- [ ] Fix any include/path errors (most likely missing `#include` lines). The classes are under `Source/TheLongNoon/{Core,Player,GameModes,Systems,UI,Data}`.

## 3. Wire the GameMode and player
- [ ] Set the project Default GameMode to `ALongNoonGameMode` (it already sets pawn `ALongNoonCharacter`, controller, and HUD).
- [ ] Reparent the template's character Blueprint to `ALongNoonCharacter` (so the template mesh/anim sits on our C++ base), OR assign the template skeletal mesh + anim BP onto a `BP_LongNoonCharacter` child of `ALongNoonCharacter`.
- [ ] On the character Blueprint, assign the Enhanced Input assets to the exposed fields: `DefaultMappingContext` = `IMC_Default`, `MoveAction`/`LookAction`/`JumpAction`/`SprintAction`/`InteractAction`/`PruneAction` = the matching `IA_*`.

## 4. Build the Sunhollow greybox map
- [ ] Create `Content/Maps/L_Sunhollow_Greybox.umap`.
- [ ] Block out the meadow + village with **Modeling Tools** (enabled).
- [ ] Add a low, warm **DirectionalLight** + a **PostProcessVolume** tuned to the golden hour (see `docs/art/style-guide.md`); keep Lumen light.
- [ ] Place a few `AGatherNode` actors (set `ItemId` to e.g. `mat_sunmoss`) and one `ABloomActor` (set `RequiredVerb = Prune`, add a `DropTable` entry).
- [ ] Set this map as Editor Startup Map and Game Default Map.

## 5. Import the data
- [ ] Import `Content/Data/*.csv` (see `docs/tech/data-authoring-guide.md`): either as DataTables (define the mirror row structs) or generate DataAssets. At minimum import Items/Tools/Recipes so crafting and gathering have content.
- [ ] Give the player's `ULongNoonReclamationComponent` a starting tool (the `tool_pruning_blade` ToolDef) via `EquipTool` or a default.

## 6. HUD (optional for first run)
- [ ] Create `WBP_HUD` deriving from `ULongNoonHUDWidget`; assign it to `ALongNoonHUD::HUDWidgetClass`. Implement `UpdateTend` / `SetInteractPrompt` in the widget graph.

## 7. Press Play
- [ ] Play-in-Editor. Verify: move (WASD/stick), camera (mouse/stick), jump, sprint; walk to a gather node and Interact to gather; walk to the Bloom and Prune; open the build/inventory hooks as wired.
- [ ] This is the first playable proof of the core loop (gather -> craft -> prune -> build). Iterate the Sunhollow from here per `docs/lore/region-1-the-sunhollow.md` and `docs/design/quest-flow.md`.

## 8. Commit the content
- [ ] With Git LFS active: `git add Content && git commit -m "Add Third Person base + Sunhollow greybox content"`.

---
## What is already done for you (committed on `feature/unreal-scaffold`)
- Full design + lore + character bible (`docs/`), art/audio direction, the GOAL backlog.
- UE5 project skeleton (`.uproject`, `Source/` C++ module, `Config/`, `Content/` layout, UE-aware `.gitignore`/`.gitattributes`).
- C++ systems (stubs with working core logic): character + camera + Enhanced Input bindings, inventory, crafting, building, tend, reclamation (trace + subdue), dialogue, quest, codex, region gating, save/load, HUD base, interactable interface, Bloom + gather actors, and the Data structs.
- Authored content: dialogue scripts, codex fragments, the cipher, endings, quest flow.
- Data CSVs for items, tools, recipes, regions, lore fragments + the authoring guide.

The gap between "this repo" and "a running game" is exactly steps 1 to 8 above: install the engine, graft the template's binary base, compile, and assemble the first map. Nothing in that gap could be done headlessly.
