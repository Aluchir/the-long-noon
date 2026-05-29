# Source/TheLongNoon — C++ module layout

Stub C++ foundation for the game module. These compile under UE 5.7 once the project
is opened/built (see `docs/tech/unreal-setup.md`). They establish the architecture from
`docs/tech/coding-standards-and-architecture.md`: C++ for systems, Blueprints for content,
data-driven via DataAssets, decoupled via the event subsystem.

```
Core/      LongNoonGameInstance      cross-level state (literacy, progression, unlocks)
           LongNoonEventSubsystem    global event dispatchers (decoupling hub)
           LongNoonDataSettings      DeveloperSettings: the content DataTable refs
Player/    LongNoonCharacter         third-person Newcomer: camera + input; drives the HUD; equips starting tool
           LongNoonPlayerController  thin; UI/save wiring later
GameModes/ LongNoonGameMode          default pawn + controller + HUD class
Systems/   LongNoonInventoryComponent    closed-economy stacks
           LongNoonReclamationComponent  non-lethal prune/still/settle/seal/quiet; stamina + durability
           LongNoonCraftingComponent     recipe craft (asset + CraftById data path); spends stamina
           LongNoonBuildingComponent     placement + gate-build hooks
           LongNoonTendComponent         Stamina/Focus/Comfort meters (no death)
           LongNoonDialogueComponent     line-advancing dialogue; LoadFromData() from the NPC roster
           LongNoonQuestSubsystem        objective tracking
           LongNoonDataRegistry          loads the DataTables; serves rows + parses packed columns
           LongNoonSaveGame + SaveService versioned slot saves
           LongNoonCodexSubsystem        found fragments + literacy-gated readability
           LongNoonRegionSubsystem       composite region gating
           BloomActor, GatherNode        reclamation target + harvestable node
UI/        LongNoonHUD + LongNoonHUDWidget  forwards Tend/interact-prompt/lore-found to UMG
Data/      ItemDef, ToolDef, RecipeDef, RegionDef, LoreFragmentDef  (UPrimaryDataAsset)
           LongNoonRowTypes          FItemRow/FToolRow/FRecipeRow/FRegionRow/FLoreFragmentRow/FNpcRow/FDialogueLineRow
```

## Status
- **All headless build-out is authored** (see `docs/GOAL.md` sections A–G, all checked). The core loop systems, data registry, and HUD wiring are in place.
- **Not yet compile/run verified** (no Unreal install on the authoring host). Expect minor include adjustments on first build in-editor.
- Designer-facing values use `UPROPERTY(EditAnywhere)`; gameplay hooks are `UFUNCTION(BlueprintCallable)`.
- Remaining `TODO`s (`LongNoonBuildingComponent` placement spawn; `LongNoonSaveService` version migration) are post-first-playable polish, not part of the headless backlog.

## Needs the UE editor (cannot be done headless)
- Compile the module; author binary content (template character, Enhanced Input assets, the Sunhollow map).
- Import `Content/Data/*.csv` as DataTables and assign them in Project Settings -> The Long Noon Data.
- See `docs/tech/first-playable-checklist.md`.

## Mapping to design docs
Inventory/economy, crafting, reclamation, tiers, progression, codex, and the regions are all
specified under `docs/design/` and `docs/lore/`. Each system here points to its spec in comments.
