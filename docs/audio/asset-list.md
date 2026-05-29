# Audio Asset List

> The production manifest for audio, derived from the [audio direction](audio-direction.md). The signature device is **one theme that hollows out** as the player descends; the cheapest path to it is to compose the theme **in stems** and derive every region state by muting/adding layers ([audio direction §7–8](audio-direction.md#7-adaptive--implementation-approach)). Implement with Unreal **MetaSounds + buses**; wrap behind an `AudioService` that takes a "theme state" + "ambience set." No voice acting (wordless vocalization) — a large scope and localization win (see [localization](../tech/localization.md)).

## Conventions
- **Priority:** P0 = first-playable slice (Sunhollow); P1 = full critical path; P2 = polish.
- Keep an **ambience-liveliness scale** parallel to the [art light-dial](../art/style-guide.md#4-color--light) so audio and visuals degrade in lockstep.
- **No grim/horror scoring** — dread is absence and wrongness-in-warmth, never stingers.

## 1. Music — the one theme, in stems (`Content/Audio/Music/`)
| Asset | Notes | Priority |
|---|---|---|
| **Main theme, authored as stems** | warm acoustic: plucked strings, gentle winds, light mallets, hand perc | **P0** |
| R1 Sunhollow state | full, warm, complete (intact melody) | P0 |
| R2 Overgrowth state | full but over-ornamented (overgrown) | P1 |
| R3 Stillworks state | hollowed — notes thinning, warmth draining | P1 |
| R4 The Hush state | a few spaced, hollow notes — nearly forgotten | P2 |
| R5 First Garden state | whole again, radiant | P2 |
| New climax color | held, glassy tone for the Gardener's presence | P2 |
| 3 ending variations | Gift Returned / Long Noon Continues / Third Way | P2 |

> All region states are **layer mutes/adds** over the one stemmed theme, not separate tracks.

## 2. Ambient soundscapes (`Content/Audio/Ambience/`)
| Set | Contents | Priority |
|---|---|---|
| R1 Sunhollow | birdsong, soft breeze, distant village life (full, alive) | P0 |
| R2 Overgrowth | dense insect/bird layers, rustling growth, dripping (almost too much life) | P1 |
| R3 Stillworks | sparse wind in empty halls, faint stylus scrape, dormant-machine hum | P1 |
| R4 The Hush | muffled near-silence, dampened sound, faint looping half-remembered lullaby | P2 |
| R5 First Garden | warm impossible stillness — beautiful, with **no life sounds** | P2 |

## 3. SFX (`Content/Audio/SFX/`)
Build a small reusable **cozy SFX library early** — these play constantly.
| Set | Contents | Priority |
|---|---|---|
| Reclamation | **Still** signature (held breath resolving); **Settle** "tucking-in"; prune/seal/quiet — all soft, non-violent | P0 |
| Gathering | snips, soft plucks, node harvest/regrow | P0 |
| Crafting | tactile thunks, gentle crafting chimes (ASMR-ish) | P0 |
| Building | placement, gate-build completion | P1 |
| UI | warm soft click/confirm/cancel language | P0 |
| Footsteps | per-surface; Newcomer's near-subliminal mortality "tell" (optional) | P1 |
| Tend feedback | low-stamina soft cue (never alarming) | P1 |

## 4. Voice / vocalization (`Content/Audio/Vox/`)
| Asset | Notes | Priority |
|---|---|---|
| Wordless vocalization set per NPC | soft gibberish/tone language, emotion-tied, paired with the text lines in `DialogueLines.csv` | P1 |
| Gardener climax vocalization | distinct calm treatment, paired with now-readable script | P2 |

> Vocalizations key off dialogue **emotion/cadence**, not words — the [cipher-name column](../lore/forgotten-script-cipher.md#2-letter-substitution-cipher-alphabet) drives spoken rhythm consistency. Universal across all languages (not localized).

## 5. Implementation assets (`Content/Audio/`)
| Asset | Notes | Priority |
|---|---|---|
| MetaSound theme graph | layer mutes/crossfades per region state | P0 |
| Submixes / audio buses | music / ambience / SFX / UI | P0 |
| `AudioService` hookup | region transition → request theme state + ambience set | P0 |
| FMOD/Wwise evaluation | only if native layering can't deliver the hollowing cleanly ([plugins](../tech/third-party-plugins.md)) | P2 (deferred) |

## 6. Open dependencies
- Compose the **one theme first**, in stems — everything else derives from it.
- The hollowing through-line needs the stem authoring done before R2–R5 states can be cut.
- Ambience liveliness must track the art light-dial; keep the two reference scales side by side.
