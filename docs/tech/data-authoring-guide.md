# Data Authoring Guide

> How the CSVs in `Content/Data/` become game data in Unreal. The CSVs are the source of truth for content; this guide explains the two ways to get them into the engine and how each column maps to the structs in `Source/TheLongNoon/Data/`.

## The CSVs
| CSV | Maps to | Key column |
|---|---|---|
| `Items.csv` | `UItemDef` | `Name` = `ItemId` |
| `Tools.csv` | `UToolDef` | `Name` = `ToolId` |
| `Recipes.csv` | `URecipeDef` | `Name` = `RecipeId` |
| `Regions.csv` | `URegionDef` | `Name` = `RegionId` |
| `LoreFragments.csv` | `ULoreFragmentDef` | `Name` = `FragmentId` |
| `NPCs.csv` | `FNpcRow` | `Name` = `NpcId` |
| `DialogueLines.csv` | `FDialogueLineRow` | `Name` = `LineId` |

The first column is always `Name`, the stable id used as the DataTable RowName and as the lookup key elsewhere (recipes reference item ids, fragments reference threads, etc.).

`NPCs.csv` and `DialogueLines.csv` are DataTable-only (`FNpcRow` / `FDialogueLineRow` in `LongNoonRowTypes.h`); there is no `U*Def` asset for them. The roster references lines by id, and the lines carry the actual text (narrative source of truth is `docs/lore/dialogue-scripts.md`). `ULongNoonDialogueComponent::LoadFromData()` reads an NPC's row and resolves its line ids into the spoken `Lines` array at runtime.

## Two ways to use them in UE

### Option A — DataTables (direct CSV import, fastest)
DataTables import straight from CSV, but they need a `FTableRowBase` struct per table (DataAssets are not CSV-importable). For each content type, define a mirror row struct, e.g.:

```cpp
USTRUCT(BlueprintType)
struct FItemRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FText DisplayName;
    UPROPERTY(EditAnywhere) EItemCategory Category = EItemCategory::Material;
    UPROPERTY(EditAnywhere) int32 Tier = 1;
    UPROPERTY(EditAnywhere) int32 MaxStack = 99;
    UPROPERTY(EditAnywhere) bool bDroppable = true;
    UPROPERTY(EditAnywhere) FText FlavorText;
};
```
Then in-editor: Import `Items.csv` as a DataTable using `FItemRow`. The column headers must match the struct property names exactly (they do). Systems read rows by RowName. This is the recommended path for bulk content.

### Option B — DataAssets (one asset per row, richer references)
The `U*Def` classes already exist as `UPrimaryDataAsset`s and support soft object references (e.g. a recipe pointing at item assets, a region pointing at a `UWorld`). For these, either:
- Hand-create one DataAsset per CSV row (use the CSV as the authoring sheet), or
- Write a small editor utility (Editor Utility Blueprint or commandlet) that reads the CSV and generates/updates the DataAssets.

Use DataAssets where you need real asset pointers (recipes -> item assets, regions -> level/world). Use DataTables where flat data is enough.

## Column encodings
- **Enums** (`Category`, `Type`) use the C++ enum entry name: `Material`, `Tool`, `Inscription`, etc.
- **Verb lists** (`Tools.csv` `Verbs`) are pipe-separated: `Seal|Prune`. Parse into `TArray<EReclamationVerb>`.
- **Recipe inputs** (`Recipes.csv` `Inputs`) are `itemId:qty` pairs separated by `;`: `mat_stillsteel:2;comp_fitting:1`. Parse into `TArray<FRecipeInput>` (resolve item ids to `UItemDef` via the registry).
- **Dialogue line lists** (`NPCs.csv` `FirstTalkLines`, `IdleLines`, `StoryBeatLines`) are pipe-separated `DialogueLines` ids: `tinker_first1|tinker_first2`. The registry resolves them to texts via `GetDialogueLines()`.
- **Text with commas** is double-quoted (standard CSV). Keep flavor text em-dash-free (house style).
- **Empty cell** = default/none (e.g. empty `RequiredTraversal` means no traversal gate).

## Cross-references to validate on import
- `Recipes.Inputs` and `Recipes.Output` item ids must exist in `Items.csv`.
- `Recipes.Output` for a tool must match a `Tools.csv` id (tools are items too at the inventory layer; keep ids consistent or map them).
- `Regions.RequiredGateBuild` ids must match the gate-build ids used by `quest-flow.md` and the building system.
- `LoreFragments.RequiredLiteracyTier` (0..4) must line up with the [forgotten-script](../lore/forgotten-script.md) tiers; threads must match the codex thread set.
- `NPCs.HomeRegion` ids must exist in `Regions.csv`; `NPCs.GrantsFragmentOnFirstTalk` must exist in `LoreFragments.csv`.
- Every line id in `NPCs` (`FirstTalkLines`/`IdleLines`/`StoryBeatLines`/`DaggerLine`) must exist in `DialogueLines.csv`, and each `DialogueLines.Speaker` must match an `NPCs` id.

## Source of truth
The narrative text behind these rows lives in `docs/lore/` (weapon-and-tool-lore.md, codex-fragments.md). When content changes, edit the lore doc and the CSV together, then re-import. A future CI check can diff CSV ids against the lore docs to catch drift.
