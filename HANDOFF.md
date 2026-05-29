# Session Handoff — The Long Noon

> Read this first when picking up on another machine (e.g. Windows 11). It tells a fresh Claude Code session, or a human, exactly where things stand and what to do next. The live Claude session does not transfer between machines; this file plus the docs carry the state.

## What this is
**The Long Noon** (working title): a cozy third-person 3D survival/crafting game with a buried narrative. A sunny world where nobody can die, nobody has family, nobody remembers why, because an alien (the Gardener) removed death as an act of mercy. The player rediscovers the buried truth by crafting into older regions. Engine: **Unreal Engine 5.7, C++ + Blueprints.** Target: Steam (Windows first).

## Branch layout
- **`main`** — the original project scaffold (one commit).
- **`feature/unreal-scaffold`** — ALL current work lives here. Check this branch out.
  - Engine pivot (Godot to Unreal), full C++ system stubs, all design/lore/data, Phase 2 data registry.

```
git checkout feature/unreal-scaffold
```

## Where state is captured
- `docs/GOAL.md` — the build-out backlog with checkboxes. **Sections A to G are all done.** The full headless build-out is complete; what remains needs the UE editor.
- `docs/BACKLOG.md` — the earlier pre-production backlog (all complete).
- `docs/WORLD_BIBLE.md`, `docs/SPRINT-1-DECISIONS.md`, `docs/characters/`, `docs/lore/`, `docs/design/`, `docs/art/`, `docs/audio/` — the full design + narrative bible.
- `docs/tech/` — engine rationale, project settings, coding standards, version control, third-party plugins, data authoring, build/run, and **`first-playable-checklist.md`**.
- `Source/TheLongNoon/` — C++ module (Core, Player, GameModes, Systems, UI, Data). See `Source/TheLongNoon/README.md`.
- `Content/Data/*.csv` — item/tool/recipe/region/lore/NPC/dialogue content tables.

## The honest caveat (important)
**None of the C++ has been compiled.** It was authored on a machine without Unreal installed, to UE5 conventions, but is unverified. Expect to fix a few `#include` lines on first build. This is normal for hand-authored UE C++.

## Exact next steps to a running game (needs UE 5.7 installed)
Follow **`docs/tech/first-playable-checklist.md`** in order:
1. Install UE 5.7 + Git LFS (`git lfs install`).
2. Generate a Third Person (C++) template project; copy its binary content (character, Enhanced Input assets, sample map) into this repo's `Content/`.
3. Compile the `TheLongNoon` module; fix any includes.
4. Set GameMode to `ALongNoonGameMode`, reparent the template character to `ALongNoonCharacter`, assign the `IMC_Default`/`IA_*` input assets.
5. Build the `L_Sunhollow_Greybox` map; place `AGatherNode`s and an `ABloomActor`; import all `Content/Data/*.csv` (seven tables now, incl. `NPCs.csv` + `DialogueLines.csv`) as DataTables (see `docs/tech/data-authoring-guide.md`) and assign them in Project Settings -> The Long Noon Data.
6. Press Play. Core loop: gather -> craft -> prune -> build. The starting tool equips from `Tools.csv`, reclamation spends stamina + wears durability, and the HUD shows Tend/interact-prompt/lore-found.

## Phase 2 status — COMPLETE (all headless work done)
Sections F and G in `docs/GOAL.md` are finished:
- **F (code TODOs wired):** crafting + reclamation spend stamina; reclamation wears tool durability (`Repair()` restores it); the character equips its starting tool by id from data; the HUD is driven from the character (Tend meters, interact prompt via focus trace, lore-found toast).
- **G (content + specs):** NPC roster + dialogue lines as data (`NPCs.csv`/`DialogueLines.csv` + `FNpcRow`/`FDialogueLineRow` + `ULongNoonDialogueComponent::LoadFromData`); `docs/design/balance-sheet.md`; `docs/art/forgotten-script-font-spec.md`; `docs/tech/localization.md` + `docs/art/asset-list.md` + `docs/audio/asset-list.md`.
All remaining work now requires the UE 5.7 editor (compile + binary content + playtest) per the checklist above.

## Not in this repo (machine-local, for awareness)
- Auto-memory lives in the local Claude config, not the repo. Key facts: this project's Ithura tracker is "The Long Noon" (LONGNOON) in the anorph workspace; the board mirrors the docs.
- No secrets are committed. `.claude/`, build outputs, and binary caches are gitignored.

## House style (keep following)
No em dashes anywhere. No AI attribution or co-author lines in commits. Cute-first-dagger-second tone for all narrative text (see `docs/lore/tone-and-writing-style-guide.md`).
