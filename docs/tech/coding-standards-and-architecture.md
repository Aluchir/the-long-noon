# Coding Standards & Architecture

> How the code and content are organized in Unreal Engine 5. Guiding idea: **C++ for systems, Blueprints for content, data-driven everything.** Designers tune via DataAssets/DataTables; gameplay-critical logic lives in typed C++ with Blueprint-exposed hooks.

## 1. Architecture overview
- **C++ base classes, Blueprint subclasses.** Core actors/components are C++ (`AActor`, `UActorComponent`, `UObject`); designers extend them in Blueprints for tuning, visuals, and quick iteration. Mark properties `UPROPERTY(EditAnywhere, BlueprintReadWrite)` and functions `UFUNCTION(BlueprintCallable)` where designers need them.
- **GameInstance** (`UTheLongNoonGameInstance`) holds cross-level state (active save slot, progression flags, script literacy). Persists across level loads, unlike GameMode.
- **GameMode / GameState / PlayerController / Pawn** per the standard UE framework. The third-person Pawn comes from the template's `Character` class, then extended.
- **Systems as ActorComponents / Subsystems:** inventory, crafting, building, reclamation, dialogue, codex, save, quest implemented as `UActorComponent`s on the player/world or as `UGameInstanceSubsystem`/`UWorldSubsystem` where global. Subsystems are the UE-idiomatic replacement for ad-hoc singletons.
- **Decoupling:** prefer **delegates / event dispatchers** and a lightweight event subsystem over hard references, so e.g. the codex reacts to "fragment found" without the world knowing the codex exists.
- **Data-driven content:** items, recipes, regions, tools, lore fragments are **DataAssets** (`UPrimaryDataAsset`) or **DataTables** under `Content/Data`. Designers add content as data, not C++.

## 2. Folder structure
```
/TheLongNoon.uproject
/Source
  /TheLongNoon
    TheLongNoon.Build.cs
    TheLongNoon.cpp / .h          (primary game module)
    /Core        GameInstance, subsystems, event bus
    /Player      character, controller, camera
    /Systems     Inventory, Crafting, Building, Reclamation,
                 Dialogue, Codex, Save, Region, Quest        (C++ components/subsystems)
    /UI          HUD, widget base classes
  TheLongNoon.Target.cs
  TheLongNoonEditor.Target.cs
/Config          Default*.ini
/Content          (binary; LFS)
  /Maps  /Characters/Player  /Environments  /Bloom
  /UI  /Audio/{Music,SFX}  /Blueprints  /Input  /Data
/Plugins
/docs            design, lore, characters, tech
```
> `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/` are generated and git-ignored. C++ subfolders (`/Core`, `/Player`, ...) are created as the systems are implemented.

## 3. C++ conventions (Unreal style)
- **Follow Epic's coding standard.** Prefixes: `A` actors, `U` UObjects/components, `F` structs, `E` enums, `I` interfaces, `T` templates.
- **UCLASS / USTRUCT / UENUM / UPROPERTY / UFUNCTION** macros for anything reflected, serialized, or Blueprint-exposed.
- Class names carry the project tag where useful (e.g. `ULongNoonInventoryComponent`); keep it consistent.
- **Memory:** UObjects are GC-managed; use `UPROPERTY()` to keep references alive; `TObjectPtr<>` for member pointers (UE5).
- **Logging:** define a project log category (e.g. `LogLongNoon`); use `UE_LOG`. No raw `printf`.
- Keep `Tick` off unless needed; prefer timers/delegates/latent actions.

## 4. Blueprint conventions
- Blueprints for **content, tuning, UI (UMG), and rapid prototyping**; not for heavy/perf-critical or networked-authority logic (that goes to C++).
- Asset naming: `BP_` Blueprints, `WBP_` widgets, `IA_`/`IMC_` input, `DA_` data assets, `DT_` data tables, `M_`/`MI_` materials, `SM_` static meshes, `SK_` skeletal meshes, `T_` textures, `A_` audio.
- Don't bury gameplay rules only a designer can read in deep Blueprint graphs — surface them as C++ data the Blueprint consumes.

## 5. Data-driven content pattern
- Define `UPrimaryDataAsset` subclasses per content type (`UItemDef`, `URecipeDef`, `URegionDef`, `UToolDef`, `ULoreFragmentDef`) with `UPROPERTY(EditAnywhere)` fields.
- Author instances as assets under `Content/Data`; systems load/index them via the **Asset Manager** at startup.
- Makes [progression](../design/progression-and-gating.md), [crafting](../design/crafting.md), [economy](../design/resource-economy.md), and [lore](../lore/codex-catalogue.md) tables tunable without recompiling.

## 6. Save/load principles
- **Versioned save schema** from day one (`SaveVersion` + migration) — QA's save-compat testing depends on it.
- Use `USaveGame` objects via a `SaveService` (GameInstance subsystem). Save **plain data** (progression flags, inventory, builds, codex/literacy, region state); rebuild actors on load rather than serializing live actors.
- Slot-based saves; Steam Cloud later ([plugins](third-party-plugins.md)).

## 7. Testing & quality
- Pure logic (economy math, save serialization, literacy gating) written to be testable via the **Automation/Functional testing** framework; keep it decoupled from heavy actor state where feasible.
- CI compiles the project and runs automation tests on PRs (production module).

## 8. House rules (apply to code artifacts too)
- **No cross-repo references** of any kind in comments, log messages, or docs.
- **No em dashes** in code comments, log strings, or docs.
- Comments explain *why*, not *what*; match surrounding style and Epic conventions.
