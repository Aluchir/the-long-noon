# Engine Rationale — why Unreal Engine 5

> The decision record for the engine choice. Confirmed: **Unreal Engine 5 (5.7)**, C++ with Blueprints. (Originally scaffolded against 5.4; standardized on 5.7 — the version installed on the build machine — before first compile.)

## The decision
**Unreal Engine 5**, third-person, full 3D, for commercial Steam release (Windows first; Linux + macOS later). C++ for systems, Blueprints for content/iteration.

## Why Unreal 5
- **Built for full 3D third-person:** ships with a production-grade **Third Person template** (character, camera, Enhanced Input, sample map) — the playable base exists on day one rather than being hand-built.
- **Proven on Steam for 3D:** Unreal powers a very large share of 3D Steam titles, AAA and indie. Strong, well-trodden shipping path with established export/packaging.
- **Best-in-class 3D toolset:** rendering (Lumen GI, Nanite), animation (Control Rig, sequencer), world-building, and the Modeling Tools for greyboxing without external DCC tools early on.
- **Blueprints + C++:** designers iterate in Blueprints; systems live in C++. Lets a small team move fast on content while keeping core logic robust and typed.
- **Marketplace (Fab):** large library of stylized characters, environments, and tools to accelerate a small team.
- **Hireable talent & tutorials:** deep pool of Unreal developers and learning material.

## The tradeoffs we accept
- **Heavier than we strictly need** for a cute/stylized look — Nanite/Lumen are more than this art demands. We mitigate by using stylized materials and only the features we need (Lumen optional; can bake/keep it light).
- **Bigger builds and longer iteration/compile times** than a lightweight engine.
- **Binary content:** all `.uasset`/`.umap` are binary, so **Git LFS is mandatory** and scenes do not text-merge (see [version control](version-control-and-branching.md)). This shapes our collaboration workflow.
- **Royalty:** 5% on gross revenue above $1M per product (a good problem to have; negligible pre-success).
- **Linux shipping** is more involved than on lighter engines; macOS is supported. Windows is the launch target regardless.

## Why not the others
- **Unity:** the close alternative — lighter, big asset ecosystem, also strong for stylized 3D on Steam. We chose Unreal for its superior out-of-the-box third-person 3D base, rendering/animation tooling, and 3D-on-Steam pedigree for a full-3D title. Unity remains a reasonable fallback if iteration speed/build weight become painful.
- **Godot 4:** free, light, and great for 2D and lighter 3D, but its 3D ecosystem is less mature and its 3D-on-Steam track record is thinner. Not the safest pick for a full-3D third-person commercial release. (We briefly scaffolded in Godot, then switched after reconfirming the full-3D/third-person/Steam priorities.)

## Key technical choices that follow
- **C++ primary for systems**, **Blueprints for content and rapid iteration.**
- **Enhanced Input** (UE5) for all input + remapping + gamepad/Steam Input.
- **Per-region baked/authored lighting** for the permanent-Long-Noon look ([Sprint 1 §5](../SPRINT-1-DECISIONS.md#5-time-of-day--the-permanent-long-noon)); Lumen used selectively, kept light to suit the stylized art and protect performance.
- **Data-driven content** via DataAssets / DataTables so designers tune without C++ (see [architecture](coding-standards-and-architecture.md)).
- **Online Subsystem Steam** for Steam integration (see [plugins](third-party-plugins.md)).

## Revisit conditions
Reconsider only if: (a) iteration/build weight materially slows the small team (→ Unity), or (b) scope shrinks so far that a lightweight engine is clearly better. Neither is expected.

## Target spec (stylized helps keep this modest)
- **Min:** entry discrete GPU, 8 GB RAM, 1080p30 (Lumen off / low).
- **Recommended:** mid discrete GPU, 16 GB RAM, 1080p60.
