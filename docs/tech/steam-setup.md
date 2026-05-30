# Steam Setup

A concise guide to finishing Steam integration for The Long Noon and shipping a
build to Steam. Targets Unreal Engine 5.7.

## Current state

The Online Subsystem Steam path is already enabled in `Config/DefaultEngine.ini`
and currently runs against Valve's public **test AppId 480** (Spacewar). This is
fine for local/dev testing of overlay, presence and SteamNetworking, but **must
be replaced before shipping**. The relevant sections, as they exist today:

```ini
[OnlineSubsystem]
DefaultPlatformService=Steam

[OnlineSubsystemSteam]
bEnabled=true
SteamDevAppId=480
bInitServerOnClient=true

[/Script/OnlineSubsystemSteam.SteamNetDriver]
NetConnectionClassName=OnlineSubsystemSteam.SteamNetConnection
```

No real AppId has been provisioned yet, and no Steamworks SDK has been dropped
into the engine — UE 5.7 ships with a bundled Steamworks SDK that backs the
`OnlineSubsystemSteam` plugin, so 480-based testing works out of the box.

## Remaining steps to ship

### 1. Provision a real AppId

- A Steamworks partner account is required (one-time USD 100 Steam Direct fee per
  app, recoverable after revenue threshold).
- In the Steamworks partner site, create the application. Steam assigns the
  numeric **AppId**. Do not invent or guess this value — it comes from
  Steamworks and is unique to the published app.
- Note any additional depot IDs Steam creates alongside the app (you typically
  get one default depot; add platform/content depots as needed).

### 2. Set the AppId in config

Replace the test AppId in `Config/DefaultEngine.ini`:

```ini
[OnlineSubsystemSteam]
bEnabled=true
SteamDevAppId=<your-real-AppId>
bInitServerOnClient=true
; Enable Steam sockets/relay only if the game does Steam-hosted multiplayer.
; The Long Noon is currently single-player, so leave this off unless/until
; networked play is added:
; bUseSteamNetworking=true
```

Also create a `steam_appid.txt` file containing only the numeric AppId next to
the executable when launching outside the Steam client (used by the Steam API to
identify the app during local runs). Do **not** ship `steam_appid.txt` in the
public depot — it is a dev convenience only.

`bUseSteamNetworking` controls whether matchmaking/voice/sockets route through
Steam's relay network. Keep it disabled for a single-player title; turn it on
(plus the `SteamNetDriver` block above) only when adding Steam-hosted sessions.

### 3. Steamworks SDK / plugin notes (UE 5.7)

- The `OnlineSubsystemSteam` plugin is included with the engine and bundles a
  specific Steamworks SDK version. Confirm that version against the one listed in
  your Steamworks partner dashboard; if Valve requires a newer SDK than the
  bundled one, update the SDK under the engine's
  `Engine/Plugins/Online/OnlineSubsystemSteam/Source/ThirdParty/Steamworks` and
  rebuild the plugin from source.
- Ensure the plugin is enabled for the project (it is implicitly active via the
  config above; verify under Edit > Plugins > Online Platform if working from the
  editor).
- The Steam client (or `steamclient` redistributable) must be present at runtime;
  during dev, having Steam running and logged in is sufficient.
- Initialization requires a valid AppId, the matching `steam_appid.txt` (dev) or
  launch via the Steam client (shipped), and a correct SDK version — a mismatch
  causes `SteamAPI_Init` to fail silently and the subsystem to fall back/disable.

### 4. Build a Shipping client

Package a **Shipping** Win64 client (no editor, no server). From the project root
(adjust engine path):

```
RunUAT.bat BuildCookRun ^
  -project="%CD%\TheLongNoon.uproject" ^
  -platform=Win64 ^
  -clientconfig=Shipping ^
  -cook -build -stage -pak -archive ^
  -archivedirectory="%CD%\Build\Shipping"
```

- Verify the staged build launches via Steam (overlay appears, presence shows the
  app name) before uploading.
- The shipped build relies on the Steam client to supply the AppId, so confirm it
  runs without `steam_appid.txt` present.

### 5. Upload via SteamPipe

Use the Steamworks **SteamPipe content builder** (`ContentBuilder` from the
Steamworks SDK) to upload the staged build.

1. Lay out the SteamPipe scripts under `ContentBuilder/scripts/`:
   - **App build script** (`app_build_<AppId>.vdf`) — references the AppId and
     lists each depot to include, plus `ContentRoot` pointing at the staged
     `WindowsClient`/archive directory and a `BuildOutput` dir for logs.
   - **Depot build script(s)** (`depot_build_<DepotId>.vdf`) — sets the DepotId,
     `ContentRoot`, and `FileMapping` (commonly `LocalPath "*"`,
     `DepotPath "."`, `Recursive "1"`).
2. Run the uploader:
   ```
   builder\steamcmd.exe +login <builder_account> +run_app_build ..\scripts\app_build_<AppId>.vdf +quit
   ```
   Use a dedicated builder account with content-upload permission, not a personal
   account with Steam Guard friction.
3. In the Steamworks partner site, the upload appears as a new **build** under the
   app. Set it live by assigning it to a branch (e.g. `default` for release, or a
   `beta`/internal branch for testing) and publishing.
4. Configure store/app settings (launch options, installation, supported OS,
   depots-per-package) in the Steamworks dashboard before going public.

## References

- Steamworks partner site: app + depot configuration, AppId assignment, builds.
- Steamworks SDK: `tools/ContentBuilder` (SteamPipe), `steamcmd`.
- UE docs: Online Subsystem Steam.
- Current project config: `Config/DefaultEngine.ini`.
