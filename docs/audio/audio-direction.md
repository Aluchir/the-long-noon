# Audio Direction

> The sonic law of The Long Noon. The audio carries the same dissonance as everything else: **warm and lovely on the surface, hollowing out underneath.** The score's central device is that the main theme *forgets itself* as the player descends, and returns whole only at the end.

## 1. North star
Cozy, melodic, acoustic-leaning warmth that you'd happily live inside for hours, sitting over a quiet wrongness. The music should feel like a sunny afternoon that has gone on a beat too long.

## 2. The signature device: the hollowing theme
There is **one main theme** (the world's "memory"). It appears in every region, and it **progressively loses notes, warmth, and instruments** as the player descends — the soundtrack itself enacting [memory erosion](../lore/metaphysics-bible.md). It returns **whole, for the first time since the start,** at the First Garden, transformed by everything the player now knows.

| Region | Theme state |
|---|---|
| R1 Sunhollow | full, warm, complete — the intact melody |
| R2 Overgrowth | full but over-ornamented (overgrown, like the Bloom) |
| R3 Stillworks | noticeably hollowed — notes thinning, warmth draining |
| R4 The Hush | reduced to a few spaced, hollow notes — nearly forgotten |
| R5 First Garden | whole again, radiant — then three ending variations |

This through-line is also an [easter egg](../lore/easter-eggs.md) for attentive listeners.

## 3. Palette & instrumentation
- **Surface (R1–R2):** warm acoustic — soft plucked strings, gentle winds, light mallets, a little hand percussion. Melodic, hummable, kind.
- **Descent (R3–R4):** the same palette **subtracted** — instruments drop out, reverb opens up, silence grows between notes. Not "darker" instruments; the *same* ones, emptied.
- **First Garden (R5):** the full palette returns, luminous, with one new color reserved for the climax (a held, glassy tone for the Gardener's presence).
- **No grim/horror scoring.** Dread comes from absence and wrongness-in-warmth, never from dissonant stingers or darkness (matches [art](../art/style-guide.md) and [tone](../lore/tone-and-writing-style-guide.md)).

## 4. Ambient soundscapes (per region)
- **R1 Sunhollow:** birdsong, soft breeze, distant village life — full and alive.
- **R2 Overgrowth:** dense insect/bird layers, rustling growth, dripping — almost too much life.
- **R3 Stillworks:** sparse wind through empty halls, the faint scrape of the One Who Counts' stylus, dormant-machine hums.
- **R4 The Hush:** muffled, near-silent, sound itself dampened; a faint half-remembered lullaby looping from a Fading One.
- **R5 First Garden:** a warm, impossible stillness — beautiful ambience with no "life" sounds in it (nothing breathes).

The ambience **degrades in liveliness** alongside the music — the world goes quiet as it forgets.

## 5. SFX
- **Reclamation:** soft, non-violent. The **still** has a signature calming tone (a held breath resolving); **settling** a growth is a gentle "tucking-in" sound. No harsh impacts (see [reclamation](../design/reclamation-combat.md)).
- **Gathering/crafting/building:** tactile, satisfying, cozy (the ASMR-ish backbone of a crafting game) — snips, soft thunks, gentle crafting chimes.
- **UI:** warm, soft, unobtrusive; a consistent cozy click/confirm language.
- **Footsteps:** per-surface; the Newcomer's footsteps may carry a near-subliminal distinct quality (mortality tell, optional).

## 6. Voice / vocalization
- **No full voice acting** (scope + the silent-protagonist design). NPCs use **gentle, wordless vocalizations** (a soft gibberish/tone language) tied to emotion, paired with text — cozy-game convention, keeps the [forgotten-language](../lore/forgotten-script.md) theme intact (the *spoken* present language is as eroded as everything else).
- The Gardener may get a distinct, calm vocalization treatment for its climax (still wordless, paired with the now-readable script).

## 7. Adaptive / implementation approach
- **Start with Unreal's MetaSounds + audio buses** (procedural/layered audio, crossfades, submixes). Powerful enough for region-based music states and the adaptive layering the "forgetting" needs.
- **Layered/adaptive design:** author the theme in stems so region states are achieved by **muting/adding layers**, not separate tracks — this makes the "hollowing" cheap and consistent.
- **Move to FMOD/Wwise only if** native layering can't deliver the adaptive forgetting cleanly (decision deferred; see [plugins](../tech/third-party-plugins.md)).
- Wrap audio behind an `AudioService` so region transitions just request a "theme state" and "ambience set."

## 8. Production notes
- Compose the **one theme first**, in stems, then derive all region variants by subtraction — this is the cheapest path to the signature device and guarantees coherence.
- Build a small, reusable **cozy SFX library** early (gather/craft/build/UI) since those play constantly.
- Keep an **ambience liveliness scale** parallel to the [art light-dial](../art/style-guide.md) so audio and visuals degrade in lockstep.
