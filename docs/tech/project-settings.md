# Project Settings

> The baseline Unreal Engine 5 project configuration and the reasoning behind it. Canonical values live in `TheLongNoon.uproject` and `Config/*.ini`; this doc explains them and lists what to set in-editor.

## 1. Project / module
- **Name:** The Long Noon (working title). **Primary game module:** `TheLongNoon` (C++).
- **Engine:** UE 5.7 (`EngineAssociation` in `TheLongNoon.uproject`).
- **Plugins enabled** (in `.uproject`): **EnhancedInput**, **OnlineSubsystemSteam**, **ModelingToolsEditorMode** (editor-only, for greyboxing).

## 2. Build targets (`Source/`)
- `TheLongNoon.Target.cs` (Game) and `TheLongNoonEditor.Target.cs` (Editor), both `BuildSettingsVersion.V5`, include-order `Unreal5_4`.
- Module deps (`TheLongNoon.Build.cs`): `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`. Add `UMG` (HUD), `GameplayTags`, etc. as systems land.

## 3. Rendering (`Config/DefaultEngine.ini`)
- **DX12** default RHI on Windows.
- **Lumen GI + reflections** available but used **selectively/lightly** — the stylized golden-hour look does not need full dynamic GI everywhere; keep it cheap to protect the modest target spec.
- **Mesh distance fields** on (needed for Lumen/shadows).
- **MotionBlur off**, **TSR/TAA** antialiasing — cozy, clean image.
- **Lighting approach:** **per-region authored/baked** golden hour, not a runtime day/night cycle ([Sprint 1 §5](../SPRINT-1-DECISIONS.md#5-time-of-day--the-permanent-long-noon)). Each region authors its mood + a "wrongness" (desaturation/angle) via post-process volumes and light setup.

## 4. Maps & modes
- Default maps/GameMode are **unset until the first map exists** (`Content/Maps`). Once the greybox Sunhollow map is created, set Editor Startup Map and Game Default Map in Project Settings (writes to `DefaultEngine.ini`).
- A `AGameModeBase` subclass (C++) defines default pawn/HUD/controller for the Sunhollow.

## 5. Input — Enhanced Input (`Config/DefaultInput.ini` + `Content/Input`)
- Uses **Enhanced Input**: input is authored as **data assets** (InputActions + InputMappingContexts) in `Content/Input`, not legacy ini mappings.
- Planned Input Actions: `IA_Move`, `IA_Look`, `IA_Jump`, `IA_Sprint`, `IA_Interact`, `IA_Prune`, `IA_Still`, `IA_OpenInventory`, `IA_OpenCodex`, `IA_OpenBuild`, `IA_OpenMap`, `IA_Pause`.
- Bound via `IMC_Default`; **all remappable** in the settings menu (UI module). Gamepad + **Steam Input** layered for controller/Deck.

## 6. Steam (platform milestone)
- `OnlineSubsystemSteam` enabled; `SteamDevAppId=480` (Steam's test AppId) until a real AppId is provisioned via a Steamworks partner account. See [plugins](third-party-plugins.md).

## 7. Packaging (`Config/DefaultGame.ini`)
- `Build=IfProjectHasCode`, Development config, pak files on. Distribution/shipping settings finalized at the Steam milestone.

## 8. To set up in-editor (checklist)
- [ ] Create the project from the **Third Person template (C++)** or open this `.uproject` and add the template character (see [unreal-setup.md](unreal-setup.md)).
- [ ] Create the greybox **Sunhollow map** in `Content/Maps`; set as Editor Startup + Game Default Map.
- [ ] Author Enhanced Input assets in `Content/Input` (actions above + `IMC_Default`).
- [ ] Create the `AGameModeBase` subclass and assign default pawn/HUD.
- [ ] Set up a per-region **PostProcessVolume** + lighting preset for the golden-hour look.
- [ ] Add the forgotten-script font and a UMG HUD widget.
- [ ] Configure Windows packaging preset (then Linux/macOS) — not committed (machine/path specific).
