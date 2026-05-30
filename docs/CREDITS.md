# Credits and Asset Licenses

The Long Noon uses the following third-party assets. All are **Creative Commons CC0 1.0**
(public domain dedication): free to use, modify, and ship commercially, with no
attribution required. We credit the authors anyway, because that is the decent thing.

Imported assets live under `Content/ThirdParty/`. The raw source archives are kept
locally under `ThirdParty/source/` (gitignored) and re-importable via the `Tools/import_*.py`
scripts.

## 3D models

- **Kenney Nature Kit** (329 models) — by Kenney (kenney.nl). CC0.
  - Source: https://kenney.nl/assets/nature-kit
  - Imported to `Content/ThirdParty/NatureKit/` via `Tools/import_nature_kit.py`.
  - Flat per-material palette colors (no textures); trees, rocks, cliffs, bridges,
    plants, crops, campfires, fences, statues, signs, and more.

- **Kenney Mini Characters** (12 humanoid characters + accessibility props) — by Kenney (kenney.nl). CC0.
  - Source: https://kenney.nl/assets/mini-characters
  - Imported to `Content/ThirdParty/Characters/` via `Tools/import_characters.py` (static meshes; superseded for NPCs by the animated pack below).

- **Kenney Animated Characters 3** (rigged character + idle/run/jump clips) — by Kenney. CC0.
  - Source: https://kenney.nl/assets/animated-characters-3 (mirror: archive.org/details/kenney_animated-characters-3)
  - Imported to `Content/ThirdParty/CharAnim/` via `Tools/import_animated_character.py` as a skeletal mesh + AnimSequences.
  - Drives the NPCs: `ALongNoonNpc` plays the looping `idle` clip (see `LongNoonNpc.cpp`).

## Audio

- **Kenney RPG Audio** (52 SFX) — by Kenney (kenney.nl). CC0.
  - Source: https://kenney.nl/assets/rpg-audio
- **Kenney Music Jingles** (86 short tracks) — by Kenney (kenney.nl). CC0.
  - Source: https://kenney.nl/assets/music-jingles

## Notes

- CC0 full text: https://creativecommons.org/publicdomain/zero/1.0/
- When swapping in bespoke art/audio later, replace the `Content/ThirdParty/` assets and
  update this file. Keep this file accurate for store-page and legal compliance.
