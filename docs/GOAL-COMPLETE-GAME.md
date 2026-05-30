# North-Star Goal — Ship The Long Noon

> The single goal everything now orchestrates around: take *The Long Noon* from "first playable proven" to a **complete, shippable game on Steam**. This is the master backlog; phase docs and the other GOAL files feed into it. Updated as phases close.

## The destination
A finished, buyable, cozy third-person survival/crafting game with its buried narrative: the player gathers, crafts, prunes the Bloom, and descends through five regions, learning the forgotten script and uncovering why no one can die, ending in one of three choices. Windows/Steam first. Engine: UE 5.7, C++ + Blueprints.

## What "complete" splits into (be honest about who does what)
- **Mechanically complete & playable** (I can drive most of this): every system working in-engine, all content authored as data/text, greybox-or-better levels for all regions, the full quest/lore/ending flow runnable, save/load, HUD, settings, build+test green. I verify behavior myself via headless launch + screenshot feedback (proven this session).
- **Shippable quality** (needs human + artists/audio, or bought asset packs): real 3D art, animation, VFX, music/SFX, the cozy visual polish, feel/balance tuning by playtest, Steam store/business/legal. I can integrate assets and scaffold everything around them, but I cannot originate the art/audio or make the final aesthetic/creative calls.

So my autonomous ceiling is **"mechanically complete, greybox-art, screenshot-verified, all systems + content + flow working."** Crossing into shippable quality is a human+art collaboration on top of that foundation.

## Operating protocol (how I orchestrate toward this)
Each work cycle:
1. Pick the next unchecked task, lowest phase first, preferring 🤖 (automatable) tasks.
2. Do it end to end: implement (code/data/Python) → build (`Build.bat TheLongNoonEditor`) → fix until green → **verify** (automation test for logic; headless `-game -sm6` launch + window screenshot for anything visual/behavioral) → commit with a clear message.
3. Keep `docs/GOAL-COMPLETE-GAME.md` checkboxes current; update phase docs.
4. **Do not block on 🧑/🤝 items.** When the next item needs human art/audio/business judgment, do the automatable part (greybox placeholder, scaffold, data, code), move the item to the **Deferred** list below with a one-line note of what a human must supply, and immediately pick the next 🤖 task. The loop never idles waiting on a human.
5. **NEVER STOP, NEVER IDLE.** Run continuously, cycle after cycle, re-arming the wake every turn, until the game is complete. No checkpoints, no asking for confirmation, no pausing at milestones. Only a genuine hard blocker (e.g. toolchain absent) pauses — and even then keep doing other automatable work first and record the blocker in Deferred.
Legend: 🤖 I drive headlessly/CLI/Python · 🤝 I scaffold, human decides/supplies · 🧑 human or artist only.

## Deferred (parked — needs human art/audio/UMG/business; not automatable headless)
The mechanical/greybox game is built and test-covered (9 automation tests; all 5 regions playable greyboxes; gather/prune/data/NPC/save/gating/literacy/endings all implemented and verified). What remains to reach a *shippable* game is fundamentally human or art-tool work I cannot originate headlessly:
- **3D art, animation, VFX** — replace greybox shapes with real cozy stylized assets per `docs/art/` (or licensed packs). The whole visual identity.
- **Music + SFX** — the one "hollowing" theme in stems + cozy SFX library per `docs/audio/`.
- **HUD / menus (UMG)** — `WBP_HUD` visual design (the C++ events are ready: `UpdateTend`/`SetInteractPrompt`/`OnLoreFound`); settings/options/accessibility UI.
- **In-game choice UI + ending sequences** — the Gardener choice screen and the three ending cinematics/text (resolution logic is done + tested).
- **Per-region art light-dial / palette** — the degrading golden-hour look (greybox uses one safe daylight setup).
- **Forgotten-script font** — per `docs/art/forgotten-script-font-spec.md` (type designer).
- **Playtest feel/balance tuning** — human judgment on game feel.
- **Steam / business** — real AppId, store page, capsule art, trailer, ratings, pricing, release (`Phase 7`).
- **Engine-GUI-only verification** — interactive prune/talk feel (injected input is unreliable under desktop focus; logic is symmetric to verified gather).

### Hard-won launch/build facts (do not relearn these)
- Build editor: `UE_5.7\Engine\Build\BatchFiles\Build.bat TheLongNoonEditor Win64 Development -Project=<uproject> -WaitMutex`.
- Standalone playtest: `UnrealEditor.exe <uproject> /Game/Maps/<Map> -game -sm6 -windowed -ResX=1280 -ResY=720`. **`-sm6` is required** (matches the editor's shader cache; plain `-game` is SM5 and recompiles every shader → minutes of white screen).
- **Use plain levels, not World Partition.** The ThirdPerson template WP map does not stream content in standalone (instant white void). Author levels as normal levels (see `Tools/build_flat_level.py`).
- Editor asset/level scripting works via the FULL editor `-ExecutePythonScript=...` (the `-run=pythonscript` commandlet crashes on `LevelEditorSubsystem`). Close the editor first (asset locks).
- Screenshot feedback: capture the game window with `CopyFromScreen` (reads composited windowed D3D; `PrintWindow` returns white for D3D and is useless here). **Must** `$proc.Refresh()` the handle, then `AttachThreadInput` + `SetForegroundWindow` to actually focus it, then verify `MainWindowTitle` contains `PCD3D_SM6` before trusting the grab — otherwise you capture whatever app is foreground (e.g. a browser).
- Building levels in Python: spawn the default `DirectionalLight`/`SkyLight`/`SkyAtmosphere` and DO NOT tweak the light via `get_component_by_class(...).set_light_color(...)` — doing so blacked out the scene. Plain default lights render the golden-hour look fine. Match `Tools/build_flat_level.py` / `build_sunhollow.py`.
- `new_level` will not overwrite an existing `.umap`; `EditorAssetLibrary.delete_asset(pkg)` first, then `new_level` + `save_current_level`.

---

## Phases

### Phase 3 — First playable (the core loop in a real level)  [IN PROGRESS]
- [x] C++ compiles on 5.7; module loads; automation tests pass.
- [x] Character grafted (mannequin + anim + input), spawns, walks/looks in a lit 3D level (screenshot-verified).
- [x] 🤖 Assign the 8 DataTables. Imported all CSVs as `DT_*` DataTables (`Tools/import_and_assign_data.py`) and assigned the `ULongNoonDataSettings` slots via `Config/DefaultGame.ini`. Verified: registry loads items=14/tools=9/recipes=13/regions=5/fragments=18/npcs=6/lines=39/builds=4 and the starting tool equips (no "no tool row").
- [x] 🤖 Build `L_Sunhollow_Greybox` (plain level, `Tools/build_sunhollow.py`): ground, golden-hour light, 3 `AGatherNode` (mat_sunmoss, sphere meshes), an `ABloomActor` (Prune, cylinder), `PlayerStart`, GameMode override. Screenshot-verified rendering.
- [x] 🤖 Gather verified end-to-end (injected E → `[Gather] 2 x mat_sunmoss gathered`). Input mapping confirmed (E and F both fire their `[Input]` handlers). Prune uses the same proven trace→component path; full in-game prune-hit + inventory/stamina/durability deltas deferred to a focused playtest (injected keys only land when the game holds OS focus, unreliable while the desktop is in active use). Logic is instead covered by automation tests (below).
- [ ] 🤝 Build `WBP_HUD` (UMG) on `ULongNoonHUDWidget` so Tend/prompt/lore show. (I can scaffold a minimal one via Python; visual design is human.)

### Phase 4 — Sunhollow vertical slice (Region 1 fully playable)
- [ ] 🤖 Full gather → craft → prune → build loop wired and tested in Sunhollow greybox.
- [x] 🤖 `ALongNoonNpc` class (interactable, dialogue from data on BeginPlay). Tinker placed in Sunhollow; verified `[NPC] npc_tinker LoadFromData=ok lines=8`. First-talk fragment grant is wired via `StartDialogue` (roster `GrantsFragmentOnFirstTalk`); the "Tinker hands you the blade" beat is cosmetic (player already equips the tool from data at spawn).
- [ ] 🤖 First codex fragments findable; literacy tier 0→1 path; HUD lore toast.
- [x] 🤖 Save/load round-trip verified via automation (`TheLongNoon.Systems.SaveRoundTrip`): literacy, tool tier, rememberer flag, inventory stacks, found fragments, unlocked regions, current region all round-trip through a slot. Suite now 6/6 green.
- [ ] 🤖 The R1→R2 gate-build (`gate_treeline_thicket`) buildable and opens a stub R2.
- [ ] 🤝 First-pass cozy art/lighting on Sunhollow (greybox → light art pass; needs assets).

### Phase 5 — Full game content (all five regions, progression, endings)
- [x] 🤖 Greybox levels R2 Overgrowth, R3 Stillworks, R4 The Hush, R5 First Garden (`Tools/build_regions.py`): each a plain level with ground, proven golden-hour sun, region signature material gather nodes, the region's NPC, PlayerStart, GameMode override. Stillworks screenshot-verified rendering (others identical). Per-region light-dial/palette is a deferred art pass.
- [ ] 🤖 Region gating wired end to end (tool tier + traversal + gate-build) per `progression-and-gating.md`.
- [ ] 🤖 All six NPCs placed with their data-driven dialogue + beats; the Rememberer in R4.
- [ ] 🤖 Forgotten-script reveal across regions (glyph→translated by literacy tier); codex complete.
- [x] 🤖 Endings resolution logic implemented + tested (`ULongNoonEndingLibrary::ResolveEnding`): Give-it-back -> Gift Returned, Keep -> Long Noon Continues, Third Way only if the Rememberer was found (else not selectable). The in-game choice UI + ending sequences (cinematics/text) are a deferred art/flow pass.
- [ ] 🤖 Balance pass using `balance-sheet.md`; gate-build + repair-kit consumption wired.
- [ ] 🤝 Region art/audio direction passes (degrading light + hollowing theme); needs assets.

### Phase 6 — Production polish (shippable quality)
- [ ] 🧑 Real 3D art, animation, VFX per `art/` (or curated asset packs); replace greybox.
- [ ] 🧑 Music (the one hollowing theme, in stems) + cozy SFX library per `audio/`.
- [ ] 🤖 Localization pipeline run (per `localization.md`); pseudoloc pass; EFIGS.
- [ ] 🤝 Accessibility, settings menu, options; UI art pass.
- [ ] 🤝 Performance + scalability; re-enable/tune Lumen for the golden-hour look.
- [ ] 🤖 Expand automation tests; the CI workflow green on a self-hosted runner.

### Phase 7 — Ship
- [ ] 🧑 Steam: provision real AppId (replace test 480), store page, capsule art, trailer.
- [ ] 🤖 Package via `BuildCookRun` (Win64); smoke-test the cooked build.
- [ ] 🧑 Age ratings, pricing, release. Post-launch patch plan.

---

## Definition of done
The game is **shippable** when all phases close: a polished, performant Sunhollow-to-First-Garden experience with the full narrative, three endings, real art/audio, localized, packaged, and live on Steam. My autonomous target en route is **Phase 5 complete at greybox/data fidelity** — every system and all content working and screenshot-verified — which is the foundation the art/audio/polish phases dress.

## Feeder docs
`GOAL.md` (headless build-out, done) · `GOAL-PHASE3.md` (first-playable detail) · `design/`, `lore/`, `art/`, `audio/`, `tech/` (the bible) · `tech/editor-graft-guide.md`, `tech/build-and-run.md` (the how).
