# Phase 3 Goal — From repo to first playable (the Sunhollow slice)

> The headless build-out (Phases 1–2, `docs/GOAL.md` A–G) is complete and committed. This phase takes the repo to a **running game you can press Play on**. It is the live to-do list; check items off as we go.

## How we work this phase (so I stop stopping)
- **I drive every 🤖 item autonomously** — no asking, no pausing between them. I run, read output, fix, re-run.
- **I only stop for 🧑 items** — things that need the GUI editor, admin rights, or a human decision I genuinely can't make. For each, I give you the *exact* one command or click to unblock, then resume on my own.
- **🤝 = you act in the editor, I prep and verify** — I stage everything, hand you a tight click-list, and check the result.
- Legend: 🤖 I do (CLI/headless) · 🧑 you do (GUI/admin) · 🤝 you click, I assist.

## This machine's reality (discovered 2026-05-29)
- ✅ **UE 5.7** installed at `C:\Program Files\Epic Games\UE_5.7` (Build.bat, UnrealBuildTool, UnrealEditor-Cmd all present). `dotnet` present.
- ❌ **No C++ toolchain** — no Visual Studio, no MSVC, no Windows SDK. **This is the hard blocker: nothing compiles without it.**
- ✅ **Standardized on UE 5.7.** `.uproject` retargeted and all docs updated. (The project was originally authored to 5.4 conventions; nothing was ever compiled, so this is a clean target, not a port.)

---

## P3.0 — Unblock the toolchain
- [ ] 🧑 **Install Visual Studio 2022 with the C++ game-dev workload** (MSVC v143 + Windows SDK). One command, run it yourself so the UAC prompt and ~10 GB download are visible. In the chat prompt type:
  ```
  ! winget install --id Microsoft.VisualStudio.2022.Community -e --override "--quiet --wait --add Microsoft.VisualStudio.Workload.NativeGame --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended"
  ```
  (Community is free. If you have Pro/Enterprise already, just add the **"Game development with C++"** workload in the VS Installer instead.)
- [x] 🤖 **Retarget engine** `TheLongNoon.uproject` `EngineAssociation` → 5.7 (and swept all docs to 5.7).
- [ ] 🤖 Once VS is present: verify the toolchain (`cl.exe` + Windows SDK detected by UnrealBuildTool).

## P3.1 — Compile the C++ module (the autonomous fix loop)
> This is where I run continuously: build → read errors → fix includes/API → rebuild, until green. The code is hand-authored to UE conventions and **has never been compiled**, so expect real fixups (missing includes, any 5.7 API deltas).
- [x] 🤖 Build `TheLongNoonEditor Win64 Development` via `Build.bat`. **Result: Succeeded (2026-05-29).**
- [x] 🤖 Fix every compile/link error. Only two fixes were needed: (1) Target.cs build settings -> `BuildSettingsVersion.V6` + `EngineIncludeOrderVersion.Unreal5_7`; (2) `Build.cs` add `PublicIncludePaths.Add(ModuleDirectory)` for the flat module layout's root-relative includes. No actual code bugs in the hand-authored C++.
- [x] 🤖 Run the automation tests headless (`UnrealEditor-Cmd ... Automation RunTests TheLongNoon`). **Both pass (2026-05-29):** `ParseRecipeInputs` and `ParseVerbs` Result=Success; the editor loads the module and the code executes, not just compiles.

**P3.1 COMPLETE.** The C++ compiles on UE 5.7, loads in-editor, and runs. Everything below this point needs the GUI editor (binary content) and is scripted in `docs/tech/editor-graft-guide.md`.

## P3.2 — Graft the Third Person template binary content
> Character mesh/anim, Enhanced Input assets, and a sample map are binary (`.uasset`) and come from Epic's template — they can't be authored headlessly.
- [ ] 🧑 Launcher → New Project → **Third Person, C++**, name `TheLongNoonTemplate` (scratch location).
- [ ] 🤝 Copy its character (`Content/ThirdPerson/...`), Enhanced Input assets (`IMC_Default`, `IA_*`), and sample map into this repo's `Content/{Characters/Player, Input, Maps}`. I'll tell you exactly which files and where.
- [ ] 🤖 Confirm Git LFS tracks them (`.gitattributes` already set) and they're staged correctly.

## P3.3 — Wire GameMode, character, and input
- [ ] 🤝 Reparent the template character BP to `ALongNoonCharacter` (keeps the mesh/anim on our C++ base).
- [ ] 🤝 Assign the input assets to the character's exposed fields (`DefaultMappingContext`, `MoveAction`…`PruneAction`). GameMode/HUD class are already set in C++.
- [ ] 🤖 Sanity-check the field names against `LongNoonCharacter.h` so your assignment list is exact.

## P3.4 — Sunhollow greybox map
- [ ] 🧑 Create `Content/Maps/L_Sunhollow_Greybox.umap`; block out meadow + village with Modeling Tools.
- [ ] 🧑 Add a low warm DirectionalLight + PostProcessVolume tuned to golden hour (`docs/art/style-guide.md`).
- [ ] 🧑 Place a few `AGatherNode` (e.g. `ItemId = mat_sunmoss`) and one `ABloomActor` (`RequiredVerb = Prune`, add a `DropTable` row).
- [ ] 🧑 Set it as Editor Startup + Game Default map.

## P3.5 — Import data + assign settings
- [ ] 🤝 Import all **7** `Content/Data/*.csv` as DataTables with their row structs (`FItemRow`/`FToolRow`/`FRecipeRow`/`FRegionRow`/`FLoreFragmentRow`/`FNpcRow`/`FDialogueLineRow`) — see `docs/tech/data-authoring-guide.md`.
- [ ] 🤝 Assign all seven in **Project Settings → The Long Noon Data**. Starting tool then equips by id from the Tools table automatically.
- [ ] 🤖 Validate CSV cross-references before import (already passing; I re-run on any data change).

## P3.6 — HUD widget
- [ ] 🤝 Create `WBP_HUD` deriving from `ULongNoonHUDWidget`; implement `UpdateTend`/`SetInteractPrompt`/`OnLoreFound`; assign to `ALongNoonHUD::HUDWidgetClass`. The character drives the events automatically.

## P3.7 — Press Play + lock it in
- [ ] 🧑 Play-in-Editor: move/look/jump/sprint; Interact to gather; Prune the Bloom; watch Tend meters + prompts on the HUD.
- [ ] 🤖 Write a first Automation test (economy/save/literacy pure-logic) so `P3.1`'s build check also runs tests and we don't regress.
- [ ] 🤝 `git add Content && git commit` the template base + greybox (LFS).

---

## Definition of done (Phase 3)
The repo compiles on 5.7, opens in-editor, and the Sunhollow greybox runs the core loop (gather → craft → prune) with a live HUD. After that, **Phase 4** = full Sunhollow vertical slice (region-1 quest flow, real art/audio pass, the monument beat) per `docs/lore/region-1-the-sunhollow.md` and `docs/design/quest-flow.md`; **Phase 5+** = remaining regions, endings, Steam packaging.

## The one thing blocking everything right now
**P3.0: install the VS2022 C++ workload.** Until `cl.exe` exists, the compile loop (P3.1) — the only big chunk I can drive autonomously — can't start. Run the winget line above; ping me when it finishes and I'll take it from there.
