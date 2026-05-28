# Reclamation ("Combat") System Design

> The game's "combat," reframed. Nothing dies, so nothing is killed. You **prune, still, and subdue** the undying Bloom and the Gardener's leftover constructs. It is gentle, satisfying, gating, and entirely non-lethal — gardening for a world that grows too much.

## 1. The reframe (and why)
In a world where [nothing can die](../lore/metaphysics-bible.md), lethal combat is impossible and off-theme. So:
- **You don't fight enemies — you tend an overgrown world.** Targets are runaway Bloom-growths and dormant constructs, not people or monsters.
- **You don't kill — you subdue.** Outcomes are *pruned, stilled (dormant), or settled (sent to sleep)* — never dead.
- **There is no death state for the player either** (see [Tend, not Survive](../SPRINT-1-DECISIONS.md#4-survival-mechanics-scope--tend-not-survive)). "Losing" an encounter means being pushed back, dropping some gathered materials, or a tool taking durability damage. You always get up.

## 2. The targets
- **Inert Bloom** — overgrowth you prune/clear for resources and passage (the bulk of activity).
- **Animate growths** — Bloom that has grown enough to move/resist; must be *settled* (R2+). Non-aggressive-feeling: they obstruct and flail, they don't murder.
- **Constructs** — the Gardener's leftover garden-tenders, dormant until disturbed; subdued, not destroyed (R3+). They read as confused caretakers, not warriors.
- **Managed/sleeping Bloom** — puzzle-like targets in R3–R4 (still to pass, seal to contain).

Every target type stays tonally gentle: this is *too much life,* not evil.

## 3. The verbs (tools → actions)
| Verb | Tool | Effect |
|---|---|---|
| **Prune** | Pruning Blade (T1) | cut back inert/light Bloom; yields materials |
| **Still** | The Stiller (T2) | temporarily dormant a patch — pass or harvest safely |
| **Settle** | Resonance Hammer (T2) | send an animate growth/construct to sleep (non-lethal "defeat") |
| **Seal** | Sealing Spade (T3) | contain managed Bloom; traversal + puzzle |
| **Quiet** | Long Tool (T4) | the refined late-game verb; barely needed in the slowing Hush |

Tools/verbs tier with regions (see [weapon tiers](weapon-and-tool-tiers.md), [gating](progression-and-gating.md)).

## 4. Encounter design
- **Gentle, readable, puzzle-leaning.** Encounters reward positioning, timing the *still*, and using the right verb — not twitch reflexes or DPS.
- **No aggression spirals / no death.** Growths obstruct, push, entangle; they never create a lethal threat. Failure = setback.
- **Region-tuned intensity** scales with Bloom tier (density, resistance, number) — the difficulty curve is *material/positional,* not lethal.
- **First Garden = none.** R5 has nothing to subdue; the toolkit goes silent on purpose (thematic payoff — see [region 5](../lore/region-5-the-first-garden.md)).

## 5. Player-tunable intensity (accessibility)
A core setting (see [Sprint 1 §3](../SPRINT-1-DECISIONS.md#3-combat-vs-cozy-ratio--70-cozy--30-reclamation)) lets players dial reclamation from **Frequent** to **Minimal / Peaceful**:
- **Frequent:** more/denser animate growths and constructs.
- **Standard:** the default ~70/30 cozy-to-reclamation split.
- **Minimal/Peaceful:** growths rarely animate/resist; the world is almost pure gardening. Progression still works (gating uses tools + builds, not kill-counts).

No one is forced into challenge; no one is denied it.

## 6. Feedback & feel
- **Tactile, soft, satisfying** — the *still* should feel like a held breath; *settling* a growth should feel like tucking something in, not a kill.
- VFX: gentle dormancy bloom-closes, soft light pulses (the Stiller), settling motes (see [art guide](../art/style-guide.md)).
- Audio: warm, non-violent impact sounds; the *still* has a signature calming tone.
- Rewards: materials, cleared passage, occasional [lore fragments](../lore/codex-catalogue.md) inside constructs.

## 7. Implementation notes
- Targets: `{type, bloomTier, animate?, requiredVerb, settleThreshold, dropTable}`.
- Verb-tool mapping data-driven; a target lists which verb(s) settle it.
- "Defeat" = transition to `dormant`/`settled` state (despawn-to-regrow), never `dead`.
- Player "downed" → respawn at last outpost/home with a small material drop; **no death screen, no permadeath, no game-over.**
- Intensity setting scales spawn density/animate-chance/resistance — a single tunable multiplier set.
