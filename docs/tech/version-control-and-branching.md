# Version Control & Branching

> How we manage the repo. Unreal makes this higher-stakes than usual: content is **binary** (`.uasset`/`.umap`), large, and does **not** text-merge. Git LFS and a "lock-and-coordinate" content workflow are mandatory, not optional.

## 1. Repository
- **Git**, hosted (GitHub or equivalent). One monorepo: `Source/` + `Content/` + `Config/` + `docs/`.
- The repo is initialized locally; remote/push pending explicit sign-off.

## 2. Git LFS (mandatory for Unreal)
Unreal content is almost entirely binary. **Install and enable LFS before committing any `Content/`:**
```
git lfs install
# Patterns are already declared in .gitattributes (*.uasset, *.umap, textures,
# audio, fbx, fonts). Just ensure LFS is installed so they are filtered.
```
- Commit `.gitattributes` (already present) so every collaborator uses LFS.
- `.gitignore` excludes generated dirs (`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`) and IDE project files (`*.sln`, `.vs/`) — these are regenerated, never committed.
- **What IS committed:** `TheLongNoon.uproject`, `Source/`, `Config/`, `Content/` (via LFS), `docs/`, `.gitattributes`, `.gitignore`.

## 3. Binary content workflow (the important part)
`.umap` and `.uasset` cannot be merged. Two people editing the same map/asset = a conflict only one can win. Mitigations:
- **File locking:** use Git LFS file locking (`git lfs lock <file>`) for maps and shared assets, or a tool like Anchorpoint/Diversion on top. Lock before editing a map; unlock on push.
- **Decompose maps with Level Instances / World Partition** so people work in different sub-levels instead of one giant `.umap`.
- **Coordinate** on shared Blueprints/maps; prefer C++/DataAsset changes (text, mergeable) where possible.
- **One-Blueprint-per-feature** discipline to reduce collisions.

> If the team grows beyond ~3 content creators, evaluate **Unreal Diff tooling** and possibly Perforce (the UE industry standard for large binary projects). Git+LFS is fine at our current scale.

## 4. Branching model (lightweight GitHub Flow)
- **`main`** — always opens in the editor and compiles. Protected.
- **`feature/<short-name>`** — all work (e.g., `feature/inventory-system`, `feature/sunhollow-greybox`).
- **`fix/<short-name>`** — bug fixes.
- Merge via PR + review; keep `main` green (opens, compiles, runs). Tag milestones (`v0.1-vertical-slice`).
- Per house rules: never push to a `prod`/production branch without explicit sign-off; default pushes go to `main` only.

## 5. Commit conventions
- Small, focused commits; present-tense imperative subject.
- Reference the Ithura card where relevant.
- **House style:** no AI attribution / co-author lines; no em dashes in messages.
- Never commit generated dirs or IDE solution files (all in `.gitignore`).

## 6. First-time setup (when leaving design phase)
```
# 1. Create the playable base (see unreal-setup.md): either generate from the
#    Third Person (C++) template into this folder, or open TheLongNoon.uproject
#    and add the template content. This produces Content/ (binary) assets.

cd survival-game
git lfs install
git add .gitattributes .gitignore TheLongNoon.uproject Source Config Content docs
git commit -m "Add Unreal 5 project base"
git push -u origin main   # only after remote is set and you confirm
```

## 7. CI (later)
A CI job (GitHub Actions or self-hosted with UE installed) that on PR to `main`: generates project files, compiles the C++ target, and runs automation tests. Catches "doesn't compile / doesn't open" before merge. UE CI needs an agent with the engine installed (heavier than typical CI); detailed in the production module.
