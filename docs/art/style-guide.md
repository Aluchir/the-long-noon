# Art Style Guide / Visual Bible

> The visual law of The Long Noon. The art carries the central dissonance: **cozy and golden on the surface, subtly wrong underneath.** The look is not just pretty cozy 3D — it is the *visible scar* of the Long Noon, and it must degrade meaningfully as the player descends.

## 1. North star
Stylized, warm, rounded, hand-crafted-feeling 3D in **permanent golden late-afternoon light.** Inviting enough to live in for hours, with a wrongness you feel before you can name. Reference *feeling*, not imitation: the coziness of Animal Crossing, the readability of low-poly stylization, the quiet unease of a too-perfect afternoon.

## 2. The pillars
1. **Golden hour is sacred.** The world is never night, never overcast-grim. All dread comes from *wrongness in the warmth,* never from darkness. (Canon rule from the [metaphysics bible](../lore/metaphysics-bible.md).)
2. **Cute first, dagger second** — visually too. Designs read adorable; the unsettling detail rewards a second look (matches the [tone guide](../lore/tone-and-writing-style-guide.md)).
3. **Readability over realism.** Stylized, low-poly-friendly forms; clear silhouettes; players always understand what they can gather, prune, build, or talk to.
4. **The look degrades on purpose.** Each region steps further from the warm baseline (see §5) — the visual half of "the forgetting."

## 3. Form language
- **Rounded, soft, hand-made.** Gentle curves, chunky readable shapes, minimal hard edges. Nothing sleek or clinical (except, pointedly, the dormant Stillworks machines).
- **Low-poly + stylized texturing** (gradient/flat/painted feel) over high-detail PBR. Keeps scope sane and the look cohesive.
- **Characters:** rounded, expressive, cozy proportions; one fixed "tell" per deep character (the Newcomer's longer shadow, the Keeper's funeral-formal dress).
- **The Bloom:** lush, friendly, slightly *too* abundant up top; stranger and more architectural as it ages. Beautiful, never gross.

## 4. Color & light
- **Base palette:** warm pastels — honeyed golds, soft greens, gentle pinks/corals, cream. High warmth, medium saturation, soft contrast.
- **Lighting:** **baked, per-region** golden hour (no runtime sun cycle — see [project settings](../tech/project-settings.md)). Long warm key light, soft ambient, gentle shadows.
- **The two dials per region** (authored, consistent steps):
  - **Light angle** — lengthens region to region (sun sinking lower, shadows stretching toward horizontal).
  - **Saturation/warmth** — subtly drains as you descend (gold → bronze → thin pale).
- These dials are the measurable "lengthening light" easter egg — screenshot-comparable across regions.

## 5. Per-region visual progression
| Region | Light | Palette | Mood |
|---|---|---|---|
| R1 Sunhollow | brightest, highest golden hour | warmest, most saturated | idyllic baseline |
| R2 Overgrowth | one step longer, amber | lushest, deep greens + gold | abundant, slightly smothering |
| R3 Stillworks | long shadows, bronze | first clear desaturation | tidy emptiness |
| R4 The Hush | near-horizontal, thin | most desaturated, pale bronze | uncanny stillness |
| R5 First Garden | gold *restored*, radiant | warm again but unnaturally perfect | beautiful, frozen, wrong |

> R5's return to brilliance is the payoff: the warmth the player has watched drain comes back at its source — and is the most unsettling of all because nothing in it breathes.

## 6. Environment & props
- **Modular region kits** (`assets/models/`) — reusable pieces per region for fast, consistent building.
- **The monuments, the Hall of Marks, the machines** are hero set-pieces — authored with extra care; they carry the story.
- **The forgotten script** appears as a real cipher font on inscriptions/props (see [forgotten script](../lore/forgotten-script.md)); letterforms are organic/botanical, *grown* not engineered. Older inscriptions crisp, recent marks degraded.
- **The home/outposts** get generous cozy decor kits (the reward layer).

## 7. VFX
- Gentle, soft, non-violent. The **still** effect = a calming light-pulse and a slow bloom-close; **settling** a growth = soft motes drifting to sleep. No blood, no death, no harsh impacts (see [reclamation](../design/reclamation-combat.md)).
- Ambient: drifting pollen/light in the warm air; the Bloom subtly breathing/growing.

## 8. UI art direction
- Clean, warm, friendly, scalable (cozy-game conventions). Soft rounded panels, the pastel palette, the script font for flavor/diegetic touches.
- Iconography readable at a glance; colorblind-safe palette (accessibility, UI module).

## 9. Production notes / scope
- **Low-poly stylized = deliberate scope control** — fewer, reusable, modular assets; baked lighting; no photoreal pipeline.
- Build **region kits** before bespoke props; greybox first (Region 1 vertical slice), then art pass.
- Maintain a **master palette + light-dial reference** so all regions degrade by consistent, intentional steps.
- Keep a shared style sheet for proportions, edge-softness, and texturing approach so multiple artists stay coherent.
