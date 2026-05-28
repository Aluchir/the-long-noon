# Third-Party Plugins & Libraries Inventory

> What we depend on beyond the engine, why, and where it goes. Keep this list short and justified — every dependency is a maintenance and licensing surface. Plugins live in `addons/`; C# libraries are NuGet packages in the `.csproj`.

## 1. Confirmed / planned

### GodotSteam (C# / GDExtension)
- **Purpose:** Steam integration — achievements, cloud saves, rich presence, Steam Input, Steam Deck support.
- **Where:** `addons/` (GDExtension) + C# bindings.
- **When:** needed for the Steam & Platform milestone, not for early gameplay. Wire after core systems exist.
- **License:** MIT (verify current).
- **Note:** abstract our usage behind a thin `SteamService` so the game runs cleanly without Steam (dev builds, non-Steam launches).

### Input remapping
- Likely built with Godot's native `InputMap` + a small custom remap UI (settings menu) rather than a plugin. Revisit only if a well-maintained plugin saves real time.

## 2. Candidates (evaluate when the need is concrete)

| Need | Candidate | Decision rule |
|---|---|---|
| Dialogue authoring | Dialogic (Godot addon) or custom | Use custom if our [dialogue](../design/) needs (literacy-aware text, codex hooks) outgrow a generic tool; otherwise adopt to save time. |
| Audio middleware | Godot native vs **FMOD**/**Wwise** | Start native (free, integrated). Move to FMOD/Wwise only if the [adaptive "forgetting" audio](../audio/audio-direction.md) demands runtime layering native can't do cleanly. Decided in the audio doc. |
| Save serialization | Godot `Resource`/JSON vs library | Prefer native + our `SaveService`. A library only if versioned migration gets unwieldy. |
| Localization | Godot native CSV translations | Native is sufficient; the forgotten-script font is a custom asset, not a plugin. |
| Unit testing | **GdUnit4** or xUnit/NUnit for pure C# | Pure-logic tests in xUnit/NUnit; GdUnit4 if we need in-engine integration tests. |

## 3. Policy
- **Justify every dependency** here before adding it (purpose, license, where, removal cost).
- **Prefer engine-native** unless a plugin clearly saves significant time or enables something native can't.
- **Wrap third-party APIs** behind our own thin service interface (e.g., `SteamService`, `AudioService`) so we can swap or stub them — and so the game runs without them in dev.
- **Pin versions**; record license for each; re-check licenses before any commercial release.
- **No dependency that pulls in cross-repo or undisclosed telemetry.**

## 4. Current dependency footprint
- **Engine:** Godot 4.3+ (.NET).
- **Runtime libs:** none beyond the .NET BCL yet.
- **Addons:** none committed yet (GodotSteam added at the platform milestone).

Keeping the footprint near-zero now is deliberate: the scaffold should open and build with just Godot + .NET.
