# Session Handoff — The Long Noon

> Read this first when picking up on another machine (e.g. Windows 11). It tells a fresh Claude Code session, or a human, exactly where things stand and what to do next. The live Claude session does not transfer between machines; this file plus the docs carry the state.

## What this is
**The Long Noon** (working title): a cozy third-person 3D survival/crafting game with a buried narrative. A sunny world where nobody can die, nobody has family, nobody remembers why, because an alien (the Gardener) removed death as an act of mercy. The player rediscovers the buried truth by crafting into older regions. Engine: **Unreal Engine 5.7, C++ + Blueprints.** Target: Steam (Windows first).

## Branch layout
- **`main`** — the live working branch; ALL work is here and pushed to `origin` (`github.com/Aluchir/the-long-noon`). Just `git checkout main` and pull.
- **`feature/unreal-scaffold`** — the old work branch (now an ancestor of `main`); historical.

```
git checkout main && git pull
```

## CURRENT STATE (2026-05-30): complete, playable, packaged cozy game
The game is **content-complete as a vertical slice and ships**. Boots to a **title screen** -> five quest-tracked regions (**Sunhollow -> Overgrowth -> Stillworks -> Hush -> First Garden**) -> **three endings** (the secret Third Way unlocks via the Rememberer). Every system is wired and verified in-engine:
- Core loop gather/craft/prune/build all advance a seeded per-region quest with a live HUD objective tracker; gates open the next region.
- **CC0 art**: all five regions reskinned with Kenney Nature Kit meshes (distinct biomes), real props (bridge gates, sign quest-boards, stone-tablet lore, statue choices), and **animated skeletal NPCs** (Kenney Animated Characters 3, looping idle). Dynamic lighting + per-region light-dial + post-process/fog.
- **CC0 audio**: interaction SFX, quest stingers, per-region BGM beds.
- **UI**: HUD (meters/prompt/objective/dialogue/toasts), pause menu, settings (volume/sensitivity), 3-slot save/load, all keyboard-driven C++ UMG.
- **Engineering**: **11 automation tests pass**; CI runs them; full `BuildCookRun` of all 6 maps succeeds and the .exe boots to the title screen. Fixed two latent bugs (GameInstanceClass was unset; NPC dialogue wasn't displayed).

Run from editor: `UnrealEditor.exe TheLongNoon.uproject /Game/Maps/L_MainMenu -game -sm6 -windowed`. Asset packs are CC0, credited in `docs/CREDITS.md`. Reusable editor automation lives in `Tools/*.py` (see `build_chain.py`, `dress_regions.py`, `relight_regions.py`, `postprocess_regions.py`, `swap_props.py`, `import_animated_character.py`).

## Where state is captured
- `docs/GOAL-COMPLETE-GAME.md` — **the north-star goal + phase backlog + the Deferred (human) list + hard-won build/launch facts. Start here.**
- `docs/GOAL.md` — the original headless build-out backlog (A–G, all done).
- `docs/GOAL-PHASE3.md` — first-playable detail (done).
- `docs/BACKLOG.md` — the earlier pre-production backlog (all complete).
- `docs/WORLD_BIBLE.md`, `docs/SPRINT-1-DECISIONS.md`, `docs/characters/`, `docs/lore/`, `docs/design/`, `docs/art/`, `docs/audio/` — the full design + narrative bible.
- `docs/tech/` — engine rationale, project settings, coding standards, version control, third-party plugins, data authoring, build/run, and **`first-playable-checklist.md`**.
- `Source/TheLongNoon/` — C++ module (Core, Player, GameModes, Systems, UI, Data). See `Source/TheLongNoon/README.md`.
- `Content/Data/*.csv` — item/tool/recipe/region/lore/NPC/dialogue content tables.

## Build / run / test (UE 5.7 installed at C:\Program Files\Epic Games\UE_5.7)
- **Compile:** `Build.bat TheLongNoonEditor Win64 Development -Project=<uproject> -WaitMutex -NoHotReloadFromIDE`
- **Tests (headless):** `UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests TheLongNoon; Quit" -unattended -nop4 -nosplash -nullrhi -testexit="Automation Test Queue Empty"` (11/11 pass)
- **Editor scripting:** full editor `-ExecutePythonScript=<Tools/x.py>` (editor CLOSED first; `Tools/build_chain.py` runs several level scripts in ONE session). The engine is single-instance: do NOT run editor/build/cook concurrently.
- **Package:** `RunUAT.bat BuildCookRun -project=<uproject> -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -nocompileeditor -map=<all 6 maps joined with +>` (see `docs/tech/build-and-run.md`).
- Screenshots collide with desktop focus; the game window only foregrounds reliably when the desktop is idle.

## What's next (remaining, all optional polish)
The mechanically + aesthetically complete game exists. Remaining items, none blocking a playable build:
1. **Walk/locomotion anim** for moving NPCs/player (idle is done; add a walk blendspace from the imported `run`/`idle` clips, or retarget a fuller CC0 anim set — Quaternius Universal Animation Library).
2. **Persist settings/saves across launches** (settings are in-session only; save slots work but wire an auto-load on Continue).
3. **Balance pass** per `docs/design/balance-sheet.md`; gate-build/repair-kit consumption.
4. **Bespoke aesthetic pass** (custom UI font/art, the forgotten-script glyph font, original score) — the only genuinely human/artist work; CC0 placeholders are in and credited.
5. **Steam**: provision a real AppId (test 480 is set), store page, packaging to SteamPipe.

## Phase 2 status — COMPLETE (all headless work done)
Sections F and G in `docs/GOAL.md` are finished:
- **F (code TODOs wired):** crafting + reclamation spend stamina; reclamation wears tool durability (`Repair()` restores it); the character equips its starting tool by id from data; the HUD is driven from the character (Tend meters, interact prompt via focus trace, lore-found toast).
- **G (content + specs):** NPC roster + dialogue lines as data (`NPCs.csv`/`DialogueLines.csv` + `FNpcRow`/`FDialogueLineRow` + `ULongNoonDialogueComponent::LoadFromData`); `docs/design/balance-sheet.md`; `docs/art/forgotten-script-font-spec.md`; `docs/tech/localization.md` + `docs/art/asset-list.md` + `docs/audio/asset-list.md`.
All remaining work now requires the UE 5.7 editor (compile + binary content + playtest) per the checklist above.

## Not in this repo (machine-local, for awareness)
- Auto-memory lives in the local Claude config, not the repo. Key facts: this project's Ithura tracker is "The Long Noon" (LONGNOON) in the anorph workspace.
- **Ithura is NOT auto-synced by the Claude session** (no Ithura tool/MCP is connected here). The board must be updated manually; treat `docs/GOAL-COMPLETE-GAME.md` + this file as the source of truth and mirror status into Ithura by hand.
- No secrets are committed. `.claude/`, build outputs, and binary caches are gitignored.

## House style (keep following)
No em dashes anywhere. No AI attribution or co-author lines in commits. Cute-first-dagger-second tone for all narrative text (see `docs/lore/tone-and-writing-style-guide.md`).
