# Build & Run (for when Unreal is installed)

> Command-line build/run reference and CI notes. These require a machine with **Unreal Engine 5.7** installed; they cannot run on a host without the engine. For the first-time in-editor path, see [first-playable-checklist.md](first-playable-checklist.md).

## Paths (set per machine)
```
UE_ROOT=/path/to/UE_5.7            # engine install
PROJECT=/path/to/survival-game/TheLongNoon.uproject
```

## Generate project files
macOS / Linux:
```
"$UE_ROOT/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project="$PROJECT" -game -engine
```
Windows (Developer Command Prompt):
```
"%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" -project="%PROJECT%" -game -engine
```

## Build the editor target (compiles the C++ module)
```
"$UE_ROOT/Engine/Build/BatchFiles/<Platform>/Build.sh" TheLongNoonEditor <Platform> Development "$PROJECT" -waitmutex
```
Where `<Platform>` is `Mac`, `Linux`, or (on Windows) use `Build.bat ... Win64`.

## Build the game target (cooked/packaged)
```
"$UE_ROOT/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun -project="$PROJECT" \
  -noP4 -platform=Win64 -clientconfig=Development -cook -build -stage -pak -archive \
  -archivedirectory=Packaged \
  -map=/Game/Maps/L_Sunhollow_Greybox+/Game/Maps/L_Overgrowth_Greybox
```
(Swap `-platform=Win64` for `Mac`/`Linux` as needed. Windows is the launch target.)

There is no `MapsToCook` in `Config/DefaultGame.ini`, so cooked maps must be listed
explicitly via `-map=` (join multiple with `+`). Add every level reachable in-game
(e.g. region transitions opened by `ALongNoonGateBuild.TargetLevel`) or `OpenLevel`
will fail in the packaged build with a missing-map error. The proven local run also
uses `-nocompileeditor` to skip the editor recompile.

## Run headless tests (Automation)
```
"$UE_ROOT/Engine/Binaries/<Platform>/UnrealEditor-Cmd" "$PROJECT" \
  -ExecCmds="Automation RunTests TheLongNoon; Quit" -unattended -nop4 -nosplash -log
```
Pure-logic tests (economy math, save versioning, literacy gating) should live under an automation spec so this stays meaningful.

## CI notes
- UE CI needs a runner with the engine installed (much heavier than typical CI). Options: a self-hosted runner with UE 5.7, or a container image with the engine.
- Suggested PR job: generate project files -> build `TheLongNoonEditor` (Development) -> run Automation tests. This catches "does not compile / does not open" before merge.
- Keep `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/` out of git (already in `.gitignore`); they are build outputs.
- Content is LFS; CI must `git lfs pull` before building if any cooked content is needed.

## Steam packaging (later, at the platform milestone)
- Provision a real Steam AppId (replace `SteamDevAppId=480` in `Config/DefaultEngine.ini`).
- Package with `BuildCookRun` per platform, then upload via SteamPipe. Detail lives in the Steam & Platform module.
