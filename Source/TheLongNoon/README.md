# Source/TheLongNoon — C++ module layout

Stub C++ foundation for the game module. These compile under UE 5.4 once the project
is opened/built (see `docs/tech/unreal-setup.md`). They establish the architecture from
`docs/tech/coding-standards-and-architecture.md`: C++ for systems, Blueprints for content,
data-driven via DataAssets, decoupled via the event subsystem.

```
Core/      LongNoonGameInstance      cross-level state (literacy, progression, unlocks)
           LongNoonEventSubsystem    global event dispatchers (decoupling hub)
Player/    LongNoonCharacter         third-person Newcomer: camera boom + Enhanced Input
           LongNoonPlayerController  thin; UI/save wiring later
GameModes/ LongNoonGameMode          default pawn + controller
Systems/   LongNoonInventoryComponent    closed-economy stacks
           LongNoonReclamationComponent  non-lethal prune/still/settle/seal/quiet
           LongNoonSaveGame + SaveService versioned slot saves
           LongNoonCodexSubsystem        found fragments + literacy-gated readability
           LongNoonRegionSubsystem       composite region gating
Data/      ItemDef, ToolDef, RecipeDef, RegionDef, LoreFragmentDef  (UPrimaryDataAsset)
```

## Status
- **Stubs with working core logic + TODOs**, not finished systems. Search `TODO` for the next implementation points.
- **Not yet compile/run verified** (no Unreal install on the authoring host). Expect minor include adjustments on first build in-editor.
- Designer-facing values use `UPROPERTY(EditAnywhere)`; gameplay hooks are `UFUNCTION(BlueprintCallable)`.

## Not yet stubbed (same patterns apply)
- `CraftingComponent`, `BuildingComponent`, `DialogueComponent`, `QuestComponent` — follow the
  `UActorComponent` pattern of `LongNoonInventoryComponent`.
- HUD/UMG widgets (add the `UMG` module dependency in `TheLongNoon.Build.cs` when starting UI).
- Bloom target actors and the gather-node system for reclamation/gathering.

## Mapping to design docs
Inventory/economy, crafting, reclamation, tiers, progression, codex, and the regions are all
specified under `docs/design/` and `docs/lore/`. Each system here points to its spec in comments.
