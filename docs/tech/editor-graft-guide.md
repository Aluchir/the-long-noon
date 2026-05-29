# Editor Graft Guide (the exact in-editor click-list)

> This is the granular companion to `docs/tech/first-playable-checklist.md`. The checklist gives the seven high-level steps to reach the first playable Sunhollow slice; this guide expands the in-editor steps into exact, ordered, menu-path-level clicks so the project owner can do them fast. Where the checklist says "reparent the character" or "import the CSVs", this doc lists every menu path, asset name, and field name. It does not repeat the rationale; read the checklist for the why.

> Prerequisite: the C++ module must build first. Do steps 1 to 2 of the checklist (generate project files, compile `TheLongNoon`) before any of the clicks below. The Blueprint reparent, the DataTable row structs, the HUD base class, and the actor classes (`AGatherNode`, `ABloomActor`) only appear in the editor once the module has compiled and the editor has been opened on a successful build. If a class or struct is missing from a picker below, the module did not compile; fix the build and reopen.

> House style: no em dashes. Keep names exact (they are case sensitive in pickers).

---

## 1. Graft the Third Person template binary content

Epic's template provides the character mesh, animation Blueprint, Enhanced Input assets, and a sample map. Generate them in a scratch project, then copy the `.uasset`/`.umap` files into this repo.

- [ ] Confirm Git LFS is active before importing any binary content: run `git lfs install` once, then `git lfs status` in the repo. `Content/**` is already tracked by `.gitattributes`. Binary `.uasset`/`.umap` must land as LFS pointers, so LFS has to be live before they enter the working tree.
- [ ] Epic Games Launcher: `New Project > Games > Third Person`. Set type to `C++`, target `Desktop`, quality `Maximum`, leave Starter Content off. Name it `TheLongNoon_Scratch` in a scratch location (NOT inside this repo). Click `Create`.
- [ ] Let the scratch project compile and open. Close it once it opens cleanly.
- [ ] In a file explorer, open the scratch project's `Content/` folder. Copy these into this repo, creating the target folders if needed:
  - Character skeletal mesh + skeleton + physics asset (template path `Content/Characters/Mannequins/...`, or `Content/ThirdPerson/...` depending on 5.7 layout) into `Content/Characters/Player/`.
  - The Animation Blueprint (`ABP_*`, e.g. `ABP_Manny` / `ABP_UnrealMannequin`) into `Content/Characters/Player/`.
  - The Enhanced Input assets: `IMC_Default` and the input actions `IA_Move`, `IA_Look`, `IA_Jump` (the template ships these; there is no template `IA_Sprint`, `IA_Interact`, or `IA_Prune`, so plan to author those, see step 3) into `Content/Input/`.
  - The sample map (`Content/ThirdPerson/Maps/ThirdPersonMap.umap` or similar) into `Content/Maps/` as a reference. We build our own `L_Sunhollow_Greybox` in step 6, so this is only a parts donor.
  - The template character Blueprint (`BP_ThirdPersonCharacter`) into `Content/Characters/Player/` (we reparent it in step 2).
- [ ] Open THIS repo's `TheLongNoon.uproject` in the editor. In the Content Browser, confirm `Content/Characters/Player`, `Content/Input`, and `Content/Maps` show the grafted assets with no "missing reference" errors. Right-click any folder and `Fix Up Redirectors in Folder` if prompted.

> If a copied asset references the scratch project's module by name, it will redirect cleanly because this repo's module is also named `TheLongNoon`. Keep this repo's `Source/`, `Config/`, and `.uproject` as-is.

---

## 2. Reparent the character Blueprint to ALongNoonCharacter

Put the template's mesh and animation onto our C++ base class.

- [ ] Content Browser: double-click `Content/Characters/Player/BP_ThirdPersonCharacter` to open the Blueprint editor.
- [ ] Top toolbar: `Class Settings`. In the Details panel under `Class Options > Parent Class`, click the dropdown and pick `LongNoonCharacter` (the C++ class `ALongNoonCharacter`).
- [ ] Rename the asset to `BP_LongNoonCharacter` (right-click in Content Browser > `Rename`), or keep the template name. Either works; the GameMode resolves the pawn by class, not by asset name.
- [ ] In the Blueprint's Components panel, select the inherited `Mesh` component. In Details: set `Skeletal Mesh` to the grafted character mesh, and set `Anim Class` to the grafted Animation Blueprint (`ABP_*`). Adjust the mesh `Location`/`Rotation` so the capsule wraps the mesh feet at the capsule base (template default is Z `-90`, Yaw `-90`).
- [ ] `Compile` then `Save`.

> Alternative (child instead of reparent): `Content/Characters/Player` right-click > `Blueprint Class`, pick `LongNoonCharacter` as parent, name it `BP_LongNoonCharacter`, then set its `Mesh > Skeletal Mesh` + `Anim Class`. Use whichever is cleaner; the rest of this guide says `BP_LongNoonCharacter`.

> The GameMode is already wired in C++ (`Source/TheLongNoon/GameModes/LongNoonGameMode.cpp` sets `DefaultPawnClass = ALongNoonCharacter`, `PlayerControllerClass`, and `HUDClass`). To make the template mesh the one that spawns, set the project Default Pawn to `BP_LongNoonCharacter`: `Window > Project Settings > Project > Maps & Modes > Selected GameMode`, set `Default Pawn Class` to `BP_LongNoonCharacter`, and confirm `Default GameMode` is `LongNoonGameMode` (or a BP child of it).

---

## 3. Assign the Enhanced Input fields (exact field names)

Open `BP_LongNoonCharacter`, select the `Self`/Class Defaults so the inherited `Input` and `Reclamation` categories show in Details. Assign each exposed `UPROPERTY` below (these are the exact field names from `Source/TheLongNoon/Player/LongNoonCharacter.h`):

Category `Input`:
- [ ] `DefaultMappingContext` = `IMC_Default`
- [ ] `MoveAction` = `IA_Move`
- [ ] `LookAction` = `IA_Look`
- [ ] `JumpAction` = `IA_Jump`
- [ ] `SprintAction` = `IA_Sprint`
- [ ] `InteractAction` = `IA_Interact`
- [ ] `PruneAction` = `IA_Prune`

Category `Reclamation`:
- [ ] `StartingToolId` = `tool_pruning_blade` (this is the default; only change it to equip a different Tools-table row on spawn).

> The template only ships `IA_Move`, `IA_Look`, and `IA_Jump`. Author the missing three before assigning them:
> - Content Browser in `Content/Input`: right-click > `Input > Input Action`, create `IA_Sprint`, `IA_Interact`, `IA_Prune`. Leave `Value Type` as `Digital (bool)` for all three (they are button presses).
> - Open `IMC_Default` and add a mapping per new action: click `+` on `Mappings`, set the action, then bind a key (suggested: `IA_Sprint` = `Left Shift`, `IA_Interact` = `E`, `IA_Prune` = `F`). Bind gamepad keys too if desired.
- [ ] `Compile` and `Save` the character Blueprint after assigning all fields.

> Movement tuning fields are also exposed if you want to override them on the BP: `WalkSpeed` (default 400), `SprintSpeed` (default 650), `InteractReach` (default 300), `InteractFocusInterval` (default 0.15). Leave at default for the first run.

---

## 4. Import the CSVs as DataTables and assign them

Import all seven content CSVs from `Content/Data/` as DataTables, each with its matching row struct, then assign them in Project Settings. The CSV column headers already match the struct property names exactly, so import is one click per file.

For each CSV:
- [ ] Content Browser: navigate to `Content/Data`. Right-click in the panel (or the folder) > `Import to /Game/Data...` (or drag the `.csv` from a file explorer into the panel).
- [ ] In the import dialog, set `Import As` = `Data Table`, then set `Row Type` (`Choose DataTable Row Type`) to the struct in the table below. Click `Apply`/`OK`.
- [ ] Name the resulting DataTable asset `DT_<name>` (e.g. `DT_Items`). Save.

Row-struct mapping (struct names from `Source/TheLongNoon/Data/LongNoonRowTypes.h`):

| CSV (`Content/Data/`) | Import Row Type | Suggested asset name |
|---|---|---|
| `Items.csv` | `FItemRow` | `DT_Items` |
| `Tools.csv` | `FToolRow` | `DT_Tools` |
| `Recipes.csv` | `FRecipeRow` | `DT_Recipes` |
| `Regions.csv` | `FRegionRow` | `DT_Regions` |
| `LoreFragments.csv` | `FLoreFragmentRow` | `DT_LoreFragments` |
| `NPCs.csv` | `FNpcRow` | `DT_NPCs` |
| `DialogueLines.csv` | `FDialogueLineRow` | `DT_DialogueLines` |

> There is also an eighth CSV, `Builds.csv` (row type `FBuildRow`), with a matching `BuildTable` slot. It is not required for the first playable loop, but import it the same way if you want the building/gate data live.

Assign the tables (the `ULongNoonDataSettings` slots from `Source/TheLongNoon/Core/LongNoonDataSettings.h`):
- [ ] `Window > Project Settings`. In the left list, find the section `The Long Noon Data` (it lives under the `Game` category; the section DisplayName is "The Long Noon Data").
- [ ] Under `Data Tables`, assign each slot to the matching DataTable:
  - [ ] `ItemTable` = `DT_Items`
  - [ ] `ToolTable` = `DT_Tools`
  - [ ] `RecipeTable` = `DT_Recipes`
  - [ ] `RegionTable` = `DT_Regions`
  - [ ] `LoreFragmentTable` = `DT_LoreFragments`
  - [ ] `NpcTable` = `DT_NPCs`
  - [ ] `DialogueLineTable` = `DT_DialogueLines`
  - [ ] (optional) `BuildTable` = `DT_Builds`
- [ ] Settings save to `Config/DefaultGame.ini` (the class is `Config = Game, DefaultConfig`). `ULongNoonDataRegistry` loads these on init, so at minimum `ItemTable`, `ToolTable`, and `RecipeTable` must be set for gathering, the starting-tool equip, and crafting to work.

---

## 5. Create the HUD widget (WBP_HUD)

Build the UMG widget on the C++ base, implement the three events the systems call, and assign it to the HUD.

- [ ] Content Browser (suggested `Content/UI`, create the folder): right-click > `User Interface > Widget Blueprint`. When prompted for the parent, pick `LongNoonHUDWidget` (the C++ class `ULongNoonHUDWidget`, which is `Abstract`, so it only appears as a parent option). Name it `WBP_HUD`.
- [ ] Design the visuals on the `Designer` tab (Tend meters, an interact-prompt text block, a lore-toast). Cozy, scalable, colorblind-safe per `docs/art/style-guide.md`.
- [ ] Switch to the `Graph` tab. In `My Blueprint > Functions` (or via right-click > `Implement Event` / the `Override` dropdown on the Functions list), implement the three `BlueprintImplementableEvent`s from `Source/TheLongNoon/UI/LongNoonHUDWidget.h`:
  - [ ] `UpdateTend(float Stamina, float Focus, float Comfort)` -> drive the meter widgets.
  - [ ] `SetInteractPrompt(const FText Prompt)` -> set the prompt text; hide it when `Prompt` is empty.
  - [ ] `OnLoreFound(FName FragmentId)` -> play the gentle lore toast.
- [ ] `Compile` and `Save`.
- [ ] Assign the widget class to the HUD. The HUD class is `ALongNoonHUD` (already set as `HUDClass` in the GameMode). Set its `HUDWidgetClass` field (from `Source/TheLongNoon/UI/LongNoonHUD.h`) to `WBP_HUD`:
  - [ ] Create a Blueprint child of `ALongNoonHUD` (Content Browser > `Blueprint Class` > parent `LongNoonHUD`, name `BP_LongNoonHUD`), set its `HUD > HUDWidgetClass` = `WBP_HUD`, then set the GameMode's `HUD Class` to `BP_LongNoonHUD` in `Project Settings > Maps & Modes`. (Using a BP child is the only way to set the `EditAnywhere` `HUDWidgetClass` default without changing C++.)

> No manual event wiring is needed beyond implementing the three events. The character drives the HUD on BeginPlay: Tend meters via `OnTendChanged`, the interact prompt via the focus-trace poll, and lore-found via the event subsystem. The widget only renders.

---

## 6. Build the L_Sunhollow_Greybox map

- [ ] `File > New Level`, pick the `Open World` or `Basic` template (Basic is fine for a greybox; keep Lumen). `File > Save Current Level As` to `Content/Maps/L_Sunhollow_Greybox`.
- [ ] Block out the meadow + village with `Tools > Modeling` (the Modeling Tools mode) or simple cube brushes. Keep it rough; this is a greybox.
- [ ] Place gather nodes: in the `Place Actors` panel search `GatherNode` (class `AGatherNode`), drag one or more into the meadow. For each, in Details under category `Gather`:
  - [ ] `ItemId` = `mat_sunmoss`
  - [ ] `Quantity` = `1` (or more)
  - [ ] `RegrowSeconds` = `30` (default)
- [ ] Place one Bloom patch: `Place Actors` search `BloomActor` (class `ABloomActor`), drag one in. In Details under category `Bloom`:
  - [ ] `RequiredVerb` = `Prune`
  - [ ] `BloomTier` = `1`
  - [ ] `DropTable`: click `+` to add one entry, set Key = an item id that exists in `Items.csv` (e.g. `mat_sunmoss`) and Value = `1`.
  - [ ] `RegrowSeconds` = `60` (default)
- [ ] Lighting and mood:
  - [ ] `Place Actors > Lights > Directional Light`. In Details, lower its `Rotation` pitch toward the horizon for a low sun, set a warm `Light Color` (golden), and a soft `Intensity`. See `docs/art/style-guide.md` for the golden-hour target.
  - [ ] `Place Actors > Volumes > Post Process Volume`. In Details, check `Post Process Volume Settings > Infinite Extent (Unbound)` so it covers the level, then tune exposure/color grading to the warm noon look. Keep Lumen GI/Reflections on.
  - [ ] Ensure a `PlayerStart` is placed on walkable ground (`Place Actors` search `Player Start`).
- [ ] `Save` the level.
- [ ] Set it as startup + default map: `Window > Project Settings > Project > Maps & Modes`:
  - [ ] `Editor Startup Map` = `L_Sunhollow_Greybox`
  - [ ] `Game Default Map` = `L_Sunhollow_Greybox`
- [ ] Confirm the level's `World Settings` (`Window > World Settings`) `GameMode Override` is `LongNoonGameMode` (or your `BP_LongNoonGameMode` child), so the pawn/controller/HUD spawn here.

---

## 7. Press Play and verify

- [ ] Toolbar `Play` (Play-in-Editor). Then verify each item:
  - [ ] Move with WASD / left stick (drives `MoveAction`).
  - [ ] Look with mouse / right stick (drives `LookAction`).
  - [ ] Jump with the bound key (`JumpAction`).
  - [ ] Sprint with `Left Shift` (`SprintAction`): speed rises from `WalkSpeed` 400 to `SprintSpeed` 650.
  - [ ] Walk up to an `AGatherNode`: the HUD interact prompt appears (`SetInteractPrompt`). Press `E` (`InteractAction`) to gather; the node becomes unavailable then regrows after `RegrowSeconds`.
  - [ ] Walk up to the `ABloomActor` and press `F` (`PruneAction`): with the `Prune` verb matching `RequiredVerb`, the patch is subdued, the `DropTable` items enter inventory, stamina is spent, and the equipped `tool_pruning_blade` durability ticks down.
  - [ ] HUD Tend meters update (`UpdateTend` for Stamina/Focus/Comfort) as you sprint and reclaim.
  - [ ] If a lore fragment is found in the slice, the lore toast fires (`OnLoreFound`).
- [ ] If the pawn spawns as the default UE mannequin without our systems, the GameMode `Default Pawn Class` is not `BP_LongNoonCharacter` (recheck step 2). If input does nothing, an `IA_*`/`IMC_Default` field is unassigned (recheck step 3). If gathering/crafting fails, a DataTable slot is unassigned (recheck step 4).

> After this passes, you have the first playable proof of the core loop (gather, craft, prune, build). Iterate the Sunhollow from here per `docs/lore/region-1-the-sunhollow.md` and `docs/design/quest-flow.md`, then commit the content with Git LFS active (see the checklist step 8).
