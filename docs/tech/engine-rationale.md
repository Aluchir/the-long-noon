# Engine Rationale — why Godot 4 (C#)

> The decision record for the engine choice. Confirmed: **Godot 4.x with C#.**

## The decision
**Godot 4 (C#), Forward+ renderer, stylized 3D, third-person.** Steam on Windows first; Linux + macOS later.

## Why Godot 4
- **Fit for the art:** the game is stylized/low-poly, not photoreal. Godot 4's Vulkan Forward+ renderer is more than sufficient for a cozy golden-hour look. We don't need Unreal-class fidelity, so its weight buys us nothing.
- **Cost & ownership:** free, open-source (MIT), **no royalties, no per-seat fees, ever.** Important for an indie-scale project and removes licensing-policy risk.
- **Cross-platform export is trivial:** one-click export templates for Windows, Linux, and macOS — directly serving the Steam-first, Linux/Mac-later plan.
- **Steam integration:** mature community bindings (GodotSteam) cover achievements, cloud saves, rich presence, Steam Input, and Deck.
- **Light builds & fast iteration:** small editor, fast open/run cycle, low-overhead exports — good for a small team.
- **C# support:** lets us write typed, testable gameplay code (our preference) while keeping the option of GDScript for quick scene glue.

## Why not the others
- **Unreal 5:** overkill for cozy stylized 3D; heavy builds; rougher Linux shipping story; 5% royalty after $1M. Its strengths (Nanite/Lumen, AAA fidelity) are irrelevant to our art direction.
- **Unity:** genuinely viable and the only serious fallback — bigger ready-made-systems ecosystem (controllers, inventory kits). We pass for now on cost/ownership grounds and recent licensing-trust concerns, but it remains the documented alternative if we later want its asset ecosystem to move faster.

## Key technical choices that follow
- **C# (.NET 8)** as the primary language; GDScript allowed for trivial scene glue.
- **Forward+** renderer (desktop-class; we are not targeting mobile/web).
- **Per-region baked lighting** rather than a runtime day/night cycle — matches the permanent-Long-Noon design ([Sprint 1 §5](../SPRINT-1-DECISIONS.md#5-time-of-day--the-permanent-long-noon)) and is cheaper and more art-directable.
- **Data-driven content** (items, recipes, regions, lore fragments) so designers tune without code (see [architecture](coding-standards-and-architecture.md)).

## Revisit conditions
Reconsider only if: (a) we pivot to photoreal 3D, (b) we need a large library of off-the-shelf 3D gameplay systems and team velocity demands it (→ Unity), or (c) a hard platform requirement emerges that Godot can't export to.

## Target spec (stylized helps)
- **Min:** integrated GPU, 8 GB RAM, 1080p30.
- **Recommended:** entry discrete GPU, 16 GB RAM, 1080p60.
