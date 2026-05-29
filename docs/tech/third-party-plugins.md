# Third-Party Plugins & Libraries Inventory

> What we depend on beyond Unreal itself, why, and where it goes. Keep the list short and justified — every dependency is a maintenance, build-time, and licensing surface. Engine plugins are toggled in `TheLongNoon.uproject` / Plugins settings; third-party code plugins live in `Plugins/`.

## 1. Enabled engine plugins (in `.uproject`)

### Enhanced Input (Epic, built-in)
- **Purpose:** all input — InputActions + InputMappingContexts, gamepad, remapping, Steam Input.
- **Where:** engine plugin; input assets authored in `Content/Input`.
- **Status:** enabled now (core to the controller).

### Online Subsystem Steam (Epic, built-in)
- **Purpose:** Steam integration — achievements, cloud saves, presence, Steam Input, Deck.
- **Where:** engine plugin; configured in `Config/DefaultEngine.ini` (`SteamDevAppId=480` until a real AppId).
- **Status:** enabled; wired at the Steam & Platform milestone, not early gameplay.
- **Note:** wrap usage behind a thin `SteamService` so the game runs without Steam (dev/non-Steam launches).

### Modeling Tools Editor Mode (Epic, built-in, editor-only)
- **Purpose:** in-editor greyboxing/blockout without an external DCC tool early on.
- **Status:** enabled (editor target only).

## 2. Candidates (evaluate when the need is concrete)

| Need | Candidate | Decision rule |
|---|---|---|
| Dialogue | Custom C++/DataAsset system, or a Marketplace dialogue plugin | Build custom if our [dialogue](../design/) needs (literacy-aware text, codex hooks) outgrow off-the-shelf; else adopt to save time. |
| Audio middleware | UE built-in (MetaSounds) vs **FMOD**/**Wwise** | Start with **MetaSounds** (native, powerful, free) for the adaptive "forgetting" score. Move to FMOD/Wwise only if MetaSounds can't deliver cleanly (decided in the [audio doc](../audio/audio-direction.md)). |
| Save/load | UE `USaveGame` + our `SaveService` | Native is sufficient; a plugin only if versioned migration gets unwieldy. |
| Inventory/3rd-person kits | Marketplace (Fab) assets | Use vetted Marketplace systems to accelerate, but own the gameplay-critical logic in C++ rather than depending on a black box. |
| Localization | UE built-in localization dashboard | Native; the forgotten-script font is a custom asset, not a plugin. |
| Testing | UE Automation / Functional Testing | Built-in; no third-party needed. |

## 3. Policy
- **Justify every dependency** here before adding it (purpose, license, where, removal cost).
- **Prefer engine-native** unless a plugin clearly saves significant time or enables something native can't.
- **Wrap third-party APIs** behind our own thin service (e.g., `SteamService`, `AudioService`) so we can swap/stub them and run without them in dev.
- **Pin plugin versions to the engine version**; re-check licenses before any commercial release. Marketplace assets: verify license allows commercial shipping.
- **No dependency that pulls in cross-repo or undisclosed telemetry.**

## 4. Current dependency footprint
- **Engine:** Unreal Engine 5.7.
- **Engine plugins:** Enhanced Input, Online Subsystem Steam, Modeling Tools (editor).
- **Third-party code plugins:** none committed yet.

Keeping the footprint minimal now is deliberate: the project should open and compile with a stock UE 5.7 install plus the built-in plugins above.
