# Version Control & Branching

> How we manage the repo. Game projects add a wrinkle: large binary assets. This covers branching, commits, and Git LFS.

## 1. Repository
- **Git**, hosted (GitHub or equivalent). The repo is currently not yet initialized for VCS — see "First-time setup" below.
- **One repo** for the whole project (code + scenes + assets + docs). Monorepo is simplest at this scale.

## 2. Git LFS (important for a game)
Binary game assets bloat Git history fast. Track them with **Git LFS** from day one:
```
git lfs install
git lfs track "*.png" "*.jpg" "*.tga" "*.wav" "*.ogg" "*.mp3" "*.glb" "*.gltf" "*.fbx" "*.blend" "*.res" "*.ttf" "*.otf"
git add .gitattributes
```
- Commit `.gitattributes` so every collaborator uses LFS.
- The `.gitignore` already excludes the `.godot/` import cache, build output, and exports (those are regenerated, never committed).

## 3. Branching model (lightweight GitHub Flow)
- **`main`** — always buildable/openable in the Godot editor. Protected.
- **`feature/<short-name>`** — all work branches off `main` (e.g., `feature/inventory-system`, `feature/sunhollow-greybox`).
- **`fix/<short-name>`** — bug fixes.
- Merge to `main` via pull request + review. Keep `main` green (opens, builds, runs).
- Tag releases/milestones (`v0.1-vertical-slice`, etc.). A `release/*` branch only if/when we approach a Steam build.

> Per house rules: never push to a `prod`/production branch without explicit sign-off; default pushes go to `main` only.

## 4. Commit conventions
- Small, focused commits; present-tense imperative subject ("Add inventory storage network").
- Reference the Ithura card where relevant.
- **House style:** no AI attribution / co-author lines; no em dashes in commit messages.
- Don't commit: `.godot/`, `bin/`, `obj/`, `.mono/`, `export_presets.cfg` (machine-specific paths), exported builds — all already in `.gitignore`.

## 5. Scene merge hygiene (Godot-specific)
- `.tscn`/`.tres` are text and *can* merge, but conflicts are painful. Mitigations:
  - **Keep scenes small and composed** (instanced sub-scenes) so two people rarely edit the same `.tscn`.
  - Coordinate on shared scenes; prefer code/data changes over large scene edits where possible.
  - Use stable UIDs (Godot 4 manages `.uid`/`uid://`); commit them.
- Treat large `.tscn` conflicts as a signal to decompose the scene.

## 6. First-time setup (when leaving design phase)
```
cd survival-game
git init
git lfs install
git lfs track "*.png" "*.jpg" "*.tga" "*.wav" "*.ogg" "*.glb" "*.gltf" "*.fbx" "*.blend" "*.ttf" "*.otf"
git add .gitattributes .gitignore project.godot TheLongNoon.csproj TheLongNoon.sln src docs assets data scenes localization addons
git commit -m "Scaffold The Long Noon (Godot 4, C#) project"
git branch -M main
git remote add origin <url>
git push -u origin main
```

## 7. CI (later)
A CI job (GitHub Actions) that, on PR to `main`: imports the project headless, builds the C# solution, and runs any tests. Catches "doesn't open / doesn't build" before merge. Detailed in the production module.
