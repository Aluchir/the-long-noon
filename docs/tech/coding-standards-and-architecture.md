# Coding Standards & Architecture

> How the code is organized and written. The guiding idea: **data-driven, signal-decoupled, system-oriented.** Designers tune content without code; systems talk through signals, not hard references.

## 1. Architecture overview
- **Autoload singletons** for cross-cutting services (`GameManager`, `EventBus`, and later `SaveService`, `RegionService`, `InventoryService`, etc.). Kept thin.
- **Systems** under `src/systems/<system>/` own their domain logic (inventory, crafting, building, reclamation, dialogue, codex, save, region, quest).
- **EventBus** (global signals) decouples systems: the codex reacts to `LoreFragmentFound` without the world scene knowing the codex exists. Prefer signals over direct cross-system calls.
- **Data-driven content:** items, recipes, regions, tools, and lore fragments are **Resources** (`.tres`) or JSON under `data/`, not hardcoded. Designers add content by authoring data, not editing C#.
- **Scenes** (`scenes/`) are composed of small, instanced sub-scenes (player, UI panels, props, region chunks) to keep them mergeable and reusable.

## 2. Folder structure
```
/project.godot, /TheLongNoon.csproj, /TheLongNoon.sln, /.gitignore
/src
  /core            GameManager, EventBus, shared services
  /player          third-person controller, camera
  /systems
    /inventory  /crafting  /building  /reclamation
    /dialogue   /codex     /save      /region   /quest
  /world           world/region runtime glue, Bloom, nodes
  /ui              HUD, menus, panels (code-behind)
/scenes
  /regions  /player  /ui  /props
/assets
  /models /textures /materials /audio/{music,sfx} /fonts /shaders
/data
  /items /recipes /regions /tools /lore
/localization      translation CSVs
/addons            third-party plugins (GodotSteam, etc.)
/docs              design, lore, characters, tech
```

## 3. C# conventions
- **.NET 8**, `Nullable` enabled, `LangVersion` latest.
- **Namespaces:** `TheLongNoon.<Area>` (e.g., `TheLongNoon.Systems.Inventory`).
- **Naming:** PascalCase for types/methods/properties/exported fields; `_camelCase` for private fields; signals as `XxxEventHandler` delegates (Godot 4 source-gen).
- **Nodes:** `public partial class Foo : Node` (Godot requires `partial`).
- **Exports:** use `[Export]` for designer-tunable values; prefer exporting a data Resource over many loose fields.
- **Signals:** declare with `[Signal]`; emit via `EmitSignal(SignalName.X, ...)`. Cross-system events go through `EventBus`.
- **No magic strings for scenes/resources:** centralize paths/UIDs; prefer `[Export] PackedScene`/`Resource` references.
- **Keep `_Process` light;** prefer signals/timers over per-frame polling.

## 4. Data-driven content pattern
- Define a `Resource` subclass per content type (e.g., `ItemDef`, `RecipeDef`, `RegionDef`, `ToolDef`, `LoreFragmentDef`) with `[Export]` fields.
- Author instances as `.tres` under `data/`.
- Systems **load and index** these at boot (a registry) and operate on them generically.
- This makes the [progression](../design/progression-and-gating.md), [crafting](../design/crafting.md), [economy](../design/resource-economy.md), and [lore](../lore/codex-catalogue.md) tables all tunable without recompiling.

## 5. Save/load principles
- **Versioned save schema** from day one (a `saveVersion` field + migration path) — QA's save-compatibility testing depends on it.
- Serialize game state (progression flags, inventory, builds, codex/literacy, region states) via `SaveService`; avoid serializing engine nodes directly — save plain data and rebuild scenes on load.
- Slot-based saves; Steam Cloud later (see [plugins](third-party-plugins.md)).

## 6. GDScript policy
- C# is primary. GDScript is allowed only for trivial scene glue/prototyping. Don't split a system across both languages.

## 7. Testing & quality
- Pure logic (economy math, save serialization, literacy gating) written to be **unit-testable** in plain C# (xUnit/NUnit), independent of the engine where feasible.
- CI builds the solution and runs tests on PRs (production module).
- Keep systems loosely coupled so they can be tested in isolation.

## 8. House rules (apply to code artifacts too)
- **No cross-repo references** of any kind in code comments, log messages, or docs.
- **No em dashes** in code comments, log strings, or docs.
- Comments explain *why*, not *what*; match surrounding style.
