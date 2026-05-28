# Project Settings

> The baseline Godot project configuration and the reasoning behind it. The canonical values live in `project.godot`; this doc explains them and lists settings to apply in-editor.

## 1. Application
- **Name:** The Long Noon (working title). **Assembly:** `TheLongNoon`.
- **Features:** `4.3`, `C#`, `Forward Plus`.
- **Main scene:** intentionally empty in the scaffold; set to the boot/main-menu scene once it exists.
- **Icon:** `res://icon.svg` (placeholder until key art).

## 2. Rendering
- **Method:** `forward_plus` (desktop Vulkan; we don't target mobile/web). Best lighting/quality for our stylized look.
- **Default environment:** `res://assets/materials/default_env.tres` — to be created; holds the baseline sky/ambient for the golden-hour mood.
- **Texture filtering:** nearest/linear per art direction (stylized may favor crisp texels); set per-material.
- **Lighting approach:** **baked, per-region** (LightmapGI or baked lighting), not a runtime sun cycle — matches the permanent Long Noon ([Sprint 1 §5](../SPRINT-1-DECISIONS.md#5-time-of-day--the-permanent-long-noon)). Each region authors its own golden-hour mood + a "wrongness" (desaturation/angle) parameter.

## 3. Display / window
- **Reference resolution:** 1920×1080.
- **Stretch mode:** `canvas_items`, aspect `expand` — UI scales cleanly across resolutions (cozy games need crisp, scalable UI).
- Fullscreen/windowed and resolution exposed in the in-game settings menu (UI module).

## 4. Physics
- **3D default gravity:** 9.8. Third-person character uses a `CharacterBody3D` controller (see [architecture](coding-standards-and-architecture.md)).

## 5. Input map (actions)
Defined as named actions (events bound in-editor / remappable in-game):
`interact`, `prune`, `still_bloom`, `sprint`, `jump`, `open_inventory`, `open_codex`, `open_build`, `open_map`, `pause`.
- Bind keyboard/mouse + gamepad for each; **all remappable** in the settings menu.
- **Steam Input** layered on top for controller support and Deck (see [plugins](third-party-plugins.md)).
- Reclamation "verbs" (still/settle/seal/quiet) map onto `prune`/`still_bloom` + context, not a key each.

## 6. Autoloads (singletons)
| Autoload | Script | Role |
|---|---|---|
| `GameManager` | `src/core/GameManager.cs` | game phase, save slot, top-level state |
| `EventBus` | `src/core/EventBus.cs` | decoupled global signals |

More autoloads (SaveService, RegionService, etc.) are added as systems land — keep the set small and justified (see [architecture](coding-standards-and-architecture.md)).

## 7. Localization
- Settings prepared for localization from the start (the [forgotten script](../lore/forgotten-script.md) is a real cipher font, so in-world text and UI must both localize). Translation CSVs live in `localization/`.

## 8. To apply in-editor (checklist)
- [ ] Create the boot/main-menu scene; set `run/main_scene`.
- [ ] Create `default_env.tres` (golden-hour baseline).
- [ ] Bind input events for all actions (kb/mouse + gamepad).
- [ ] Set up export presets for Windows (then Linux/macOS) — not committed (machine-specific).
- [ ] Configure LightmapGI defaults for baked per-region lighting.
- [ ] Add the forgotten-script font to `assets/fonts/` and register it.
