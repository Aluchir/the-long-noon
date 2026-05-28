# World Bible & Game Design Document

> **Status:** Draft v0.1 — foundation document. Everything (art, code, audio, marketing) hangs off this.
> **Working title:** _The Long Noon_ (placeholder)
> **Genre:** Cozy third-person survival / crafting / exploration with a buried narrative.
> **Platform:** Steam (Windows first), Linux + macOS later.
> **View:** Third-person, 3D, stylized (non-photorealistic).

---

## 1. The one-sentence pitch

A sunny, charming world where nobody can die, nobody has family, and nobody remembers why — and as you craft your way into older and older regions, you slowly uncover what the alien took from everyone, including the meaning of the warmth you've been feeling the whole time.

---

## 2. The central dissonance (the heart of the game)

This is the single most important idea. Protect it in every decision.

**On the surface:** a feel-good, casual, sunny survival-crafter. Pastel light, gentle music, friendly people, satisfying gathering and building. You could play 30 hours and just enjoy a cozy game.

**Underneath:** that warmth is the *scar tissue of a catastrophe*. The world is pleasant because everyone has forgotten how to grieve. The cuteness is not a contrast to the horror — **it is the horror, dressed up.**

The game never forces the dark reading. It rewards the curious. Players who dig — into ruins, dialogue, item descriptions, the easter eggs — assemble the real story. Everyone else gets a lovely cozy game and goes home happy. (Think _Animal Crossing's_ skin over _Outer Wilds'_ soul.)

---

## 3. Metaphysics — what actually happened

### The Long Noon
There was a day the sun stopped moving. It hangs now at a permanent gentle late-afternoon golden hour — never noon's glare, never night's cold. This is **why the world looks the way it does**: the sunny pastel aesthetic is literally the visible wound. Locals just call it "the day," because there is only one.

### The Visitor (the alien)
Long ago something arrived. People call it different soft names — **the Gardener**, **the Warden**, **the Kind One** — because the few fragments of memory that survive remember it as *gentle*. It did not invade. It **tended.** It looked at humanity the way a gardener looks at a garden full of dying things, and it did the one merciful thing it understood: it **removed death.**

It did not understand that death was load-bearing.

### What immortality erased
With no death:
- **No families.** No birth, no lineage, no children, no elders. People simply persist. The words "mother," "son," "ancestor" exist only as dead vocabulary in old texts nobody reads.
- **No romance, no partners.** Bonds built on "I will lose you" lost their gravity. People are friendly, fond, casual — and fundamentally untethered. Affection without stakes flattens into pleasantness.
- **No memory.** This is the crucial one. Immortal minds were never built to hold centuries. Without death there's no generational hand-off of memory, so history doesn't accumulate — it **erodes.** Everyone is old beyond counting and remembers almost nothing. That is *why the truth is buried and delivered through lore, not exposition* — the characters genuinely cannot remember. **The player rediscovers what the world forgot.**

### The Bloom (why there are weapons — see §7)
The same force that stopped death also stopped decay. Nature can't die either, so it never stops growing. Plants, fungi, and the alien's own seeded growths pile up endlessly into the **Bloom** — beautiful, overgrown, choking. Reclaiming land from the Bloom is the world's central physical labor, and it's why you carry tools that look like weapons.

---

## 4. Themes (the compass)

- **Meaning requires loss.** A perfect, painless world that has quietly become hollow.
- **Forgetting as mercy and as theft.** The Gardener took the pain; it also took the people.
- **Kindness can be a catastrophe.** The villain, if there is one, loves you.
- **Rediscovery over nostalgia.** You can't go back. You can choose what to carry forward.

The ending question the whole game builds toward: *if you could give death back to the world — and with it grief, family, love, and endings — would you?* (Reserve the answer for design later; it's the spine of the third act.)

---

## 5. Society & characters

### How people are
Warm but shallow. Helpful, incurious, present-tense. They have hobbies, not histories. They'll tell you "I've always run this stall" and mean *as long as I can remember*, which is about a decade — everything before fogs out. Nobody asks where the player came from, because nobody asks anyone anything.

### Character archetypes (the casual surface)
- **The Tinker** — your crafting mentor. Cheerful, tells you what to build. Has rebuilt the same workshop a hundred times and finds it new each time.
- **The Forager** — knows every plant in the nearest Bloom, none of their names older than her memory.
- **The Keeper** — tends a "monument" (a ruin) and has no idea what it commemorates. Polishes it daily, weeps sometimes, doesn't know why.

### The deep characters (the buried surface)
- **The One Who Counts** — an NPC quietly making tally marks on a wall, millions of them. Won't say what for. (Easter egg: the count matches the number of people who existed *before* the Long Noon. They're the last person grieving, and they don't know that's what they're doing.)
- **A fragment of the Gardener** — encountered late, in the oldest region. Not hostile. Gentle, confused, proud of its garden, asking the player if everyone is happy now. The emotional climax lives here.
- **The Rememberer** — the one person whose memory didn't erode, hiding because remembering everything while immortal is unbearable. Optional, deep, the key to the true ending.

> **Design rule:** every "cute" NPC gets one detail that, if you stop and think, is devastating. Cuteness first, dagger second.

---

## 6. Regions (the story-delivery engine)

Regions are gated by gear/crafting tier. **Progression is descent into truth** — newer regions are pleasant and shallow; older regions are closer to the Long Noon's epicenter and hold deeper, darker lore. Geography = chronology = revelation.

| # | Region | Gate (gear/craft tier) | Surface mood | Buried layer |
|---|--------|------------------------|--------------|--------------|
| 1 | **The Sunhollow** | Starting area | Idyllic village, gentle meadows | Tutorial. The monument nobody understands. |
| 2 | **The Overgrowth** | Basic Bloom-cutting tools | Lush, colorful, slightly too alive | First hints decay has stopped; Bloom is unnatural. |
| 3 | **The Stillworks** | Mid-tier crafted gear + building | Abandoned-yet-tidy old infrastructure | Machines built *to end things*, now purposeless. |
| 4 | **The Hush** | Advanced gear, climbing/sealing tools | Quiet, beautiful, wrong | Where memory erodes fastest; NPCs barely coherent. |
| 5 | **The First Garden** | Top-tier gear + key items | Achingly lovely, golden, frozen | The Gardener's landing point. The truth. The choice. |

(Add side-regions/biomes for variety later — caves, water, vertical zones — but the **5-beat spine above carries the story.**)

---

## 7. Weapons & crafting — solving "why fight if no one dies?"

A hard design problem, and a feature once solved:

**Nothing you meet wants to kill you, and nothing you meet *can* die.** So combat is reframed:

- **Your "weapons" are reclamation tools** — for cutting, stilling, and pruning the **Bloom** and the alien's overgrown constructs. A scythe, a resonance-hammer, a pruning-blade, a "stiller" that briefly returns a patch of world to dormancy.
- **"Enemies"** are not monsters but **the world's runaway life** — animate Bloom-growths, defensive garden-constructs left by the Gardener, swarms of things that can't stop multiplying because they can't die. You don't kill them; you **subdue / unmake / send dormant.**
- **Weapon progression gates regions** naturally: thicker, older Bloom needs better tools, which need better crafting, which needs materials from the previous region. The loop is honest and cozy: *gather → craft → build → reach the next region → learn → repeat.*
- **Lore in the gear:** every weapon and tool has a flavor description that's another puzzle piece. (e.g., a blade "forged from a Stillworks part labeled in a script no one can read" — the player, slowly, learns the script.)

This keeps the game **non-lethal, gentle, and cozy** while preserving satisfying weapon/gear progression and clean region-gating. Weapons as gardening, not war.

### Core crafting/building pillars
- **Gather** raw materials from each region's Bloom.
- **Craft** tools, gear, and decor at workbenches (tiered).
- **Build** your base/home and region outposts (unlocks fast travel + functions).
- **Upgrade** to break into the next region.

---

## 8. Core gameplay loop

**Moment-to-moment:** explore a sunny 3D region → prune the Bloom / gather → return to base → craft & build → unlock the next gear tier.

**Session-to-session:** reach a new region → meet its NPCs → soak the surface mood → (optionally) dig up its buried lore layer.

**Whole-game arc:** the cozy loop carries you *physically* deeper while the story carries you *emotionally* deeper, until the two converge at the First Garden and the final choice.

---

## 9. Art & audio direction

- **Art:** stylized 3D, soft pastel palette, rounded forms, golden permanent-afternoon lighting (the Long Noon, baked into the look). Cute, readable, low-poly-friendly to keep asset cost and scope sane.
- **Tells:** as you go deeper, the palette desaturates *very* subtly and the light angle lengthens — players feel "wrong" before they can name it.
- **Audio:** warm, gentle, melodic on the surface. Deeper regions: same melodies, slowed, hollowed, with notes missing — the soundtrack itself "forgetting."
- **UI:** clean, friendly, cozy-game conventions.

---

## 10. Technical requirements (Steam-first)

### Engine
- **Primary recommendation: Godot 4 (C#).** Free, MIT, light builds, one-click export to Windows/Linux/macOS, Steam via **GodotSteam**.
- **Alternative: Unity** if we want the larger ready-made third-person/inventory ecosystem (Steamworks.NET for Steam).
- _Decision pending final confirmation._

### Target platforms
1. **Windows (x64)** — launch target, Steam.
2. **Linux (x64)** — later (Godot export + Proton fallback).
3. **macOS (Apple Silicon + Intel)** — later; requires code-signing + notarization for distribution.

### Steam integration checklist (for later)
- Steamworks SDK + GodotSteam/Steamworks.NET wrapper.
- Steam App ID (requires Steamworks partner account, ~$100 one-time per app).
- Achievements, cloud saves, rich presence.
- Steam Input for controller support.
- Steam Deck verification pass (Linux/Proton).

### Baseline tech systems to build (rough)
- Third-person character controller + camera.
- Save/load (slot-based, JSON or binary).
- Inventory + crafting + building systems.
- Region streaming / scene management + fast travel.
- Dialogue + lore/codex system (delivers the buried story).
- Tool/"combat" interaction system (prune/still/subdue).
- Settings, input remapping, accessibility (subtitles, colorblind-safe palette, scalable UI).

### Target spec (aim, stylized/low-poly helps)
- **Min:** integrated GPU, 8 GB RAM, 1080p30.
- **Recommended:** entry discrete GPU, 16 GB RAM, 1080p60.

---

## 11. Suggested roadmap (after this doc)

You chose **doc-first**, so code comes later. When ready, the phase order:

1. **Lock the World Bible** (this doc) — pin lore, the final choice, region count, weapon list.
2. **Engine confirmation + repo scaffold** — empty Godot project, version control, folder structure.
3. **Greybox vertical slice** — Region 1 (Sunhollow): move, gather, one craft, one Bloom-prune. Proves the loop.
4. **Core systems pass** — inventory/crafting/building/save.
5. **Art & audio direction prototype** — nail the cozy look + the "forgetting" audio.
6. **Region-by-region production** — build the 5-beat spine.
7. **Steam release prep** — partner account, store page, achievements, Deck pass, demo.

---

## Open questions to resolve next

- **The final choice:** is giving death back the literal ending mechanic? Multiple endings?
- **Player identity:** who is the player character? (Strong option: the player is *new* — the first being to exist since the Long Noon, which is why you can still feel curiosity and grief when others can't. That explains the whole game.)
- **Tone of the alien:** purely tragic-benevolent, or is there a darker turn?
- **How much combat** vs pure cozy gathering — what's the ratio?
- **Title.** _The Long Noon_ is a placeholder.
- **Scope reality check:** solo, small team, or hiring? Determines region count and 3D asset budget.
