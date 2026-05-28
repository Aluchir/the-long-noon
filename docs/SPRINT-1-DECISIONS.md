# Sprint 1 — Foundation & Gating Decisions (recommended answers)

> First-draft proposals for the gating cards in **Sprint 1: Foundation & Gating Decisions**.
> These are recommendations to react to, not final law. Each maps to an Ithura card.

---

## 1. Central premise & player identity — **the Newcomer**

**Decision:** The player character is **the first genuinely new consciousness to exist since the Long Noon.** Not reborn, not arrived from elsewhere — simply *new*, the first new person in an age where no one is born and no one dies.

**Why this is the keystone:**
- It explains, in-fiction, why *only the player* feels curiosity, attachment, unease, and the faint seed of grief — the very things the immortal population lost. Everyone else has had those feelings sanded smooth by centuries; the Newcomer arrives with them intact.
- It explains the entire game structure: **the player rediscovers what the world forgot** because the player is the one mind young enough to still *want* to know.
- It quietly sets up the ending: the Newcomer is the one variable the Gardener's perfect, painless world didn't account for.

**The buried dagger (revealed late):** the Newcomer is **mortal.** They can age. They will, one day, die. They are the only person in the world who can — and the world has no idea what is walking among it. The cozy game you've been playing is the story of the last mortal being, slowly realizing it.

See [`characters/player-character.md`](characters/player-character.md) for the full treatment.

---

## 2. The final choice & endings

**Decision:** the spine of Act 3 is a single question posed by the Gardener fragment: *now that you understand what was taken, do you want it back?* Three endings:

1. **The Gift Returned (Death restored).** The player asks the Gardener to undo its mercy. Death, and with it grief, family, love, lineage, and endings, returns to the world. Bittersweet: the world becomes mortal again — it will hurt, and it will mean something. Framed neither as clearly right nor wrong.
2. **The Long Noon Continues (Stasis kept).** The player chooses to leave the world as it is — painless, pleasant, hollow. The Newcomer keeps the truth to themselves. The only one who remembers, carrying it alone. (Echoes the Rememberer — see ending condition below.)
3. **The True Ending — The Third Way.** Unlocked only if the player has found the **Rememberer** and assembled enough lore fragments. A third option neither the Gardener nor the world imagined: not restoring death wholesale, not preserving the lie, but *giving the world back its memory* — letting people choose for themselves, person by person, whether to remember and become mortal again. Hardest to reach, most hopeful.

**Open for you:** is ending 3 the "canon" ending, or do we keep all three equally weighted? Recommendation: keep them equal in tone; let the player's journey decide.

---

## 3. Combat-vs-cozy ratio — **~70% cozy / 30% reclamation**

**Decision:** this is a cozy game first. Combat ("reclamation") is present, satisfying, and gates progression, but it is **never the point and never lethal.**

- **Default split:** roughly 70% of moment-to-moment play is gathering / crafting / building / exploring / talking; ~30% is pruning, stilling, and subduing the Bloom.
- **No death, for anyone — including the player** (see §4). Reclamation "failure" means being pushed back, dropping some gathered resources, or a tool needing repair — never a death screen.
- **Player-tunable intensity:** an accessibility/comfort setting lets players dial reclamation encounters from "frequent" to "minimal / peaceful mode," because the cozy crowd and the challenge crowd both deserve the world.
- **Tone of combat:** gentle and almost gardening-like. You are *tending* an overgrown world, not fighting a war.

---

## 4. Survival mechanics scope — **"Tend," not "Survive"**

**Decision:** drop classic survival-death loops (no starving, no health bar that kills you) — they directly contradict the premise that *nobody can die.* Replace them with non-lethal **upkeep meters** that create cozy rhythm without threat:

- **Stamina / Focus** — spent on sprinting, pruning, heavy crafting; recovers by resting at a fire/home or eating. Hitting zero means you slow down and must rest, never die.
- **Comfort** — a soft, optional meter raised by good food, a decorated home, warmth. High comfort grants gentle buffs (faster crafting, more lore insight). It's a reward system, not a punishment system.
- **Tool durability** — tools wear and need repair/upgrading. This is the real "survival" pressure and it feeds the crafting loop and region gating.
- **The Newcomer's secret meter (late game):** the player, alone, has a hidden **mortality** that the others lack — a thematic, not punishing, system surfaced only when the story is ready to reveal it.

**Net:** the tension is "keep your kit and your home in good shape to reach the next region," not "don't die."

---

## 5. Time-of-day — **the permanent Long Noon**

**Decision:** **no day/night cycle.** The sun is frozen at gentle golden late-afternoon everywhere — this is the world's defining wound and its cozy aesthetic, so we commit to it fully.

- **"Time" is measured by activity, not clock:** the Bloom regrows, NPC routines loop, crafting takes "a while" — but the sky never changes in a given region.
- **Region-by-region light is the storytelling dial.** Each deeper region shifts the *quality* of the Long Noon: the Sunhollow is warm and bright; the light grows longer, more amber, more shadow-stretched, and subtly desaturated as you descend toward the First Garden. Players *feel* something is wrong before they can name it. This is the visual half of "the forgetting."
- **Sleeping** exists as a comfort/save action and a way to fast-forward Bloom regrowth — not to pass to "night," because there is none.

**Implication for art/code:** lighting is authored per-region (a baked golden-hour mood + a "wrongness" parameter), not a runtime sun cycle. Cheaper to build and more controllable. Good news for scope.

---

## 6. Core gameplay loop (locked spec)

**Moment-to-moment:** explore a sunny region → prune/gather from the Bloom → return to base → craft & build → upgrade gear.
**Session-to-session:** new gear tier → break into the next region → meet its people → soak its mood → (optionally) dig its buried lore.
**Whole-game:** the cozy loop carries you *physically* deeper while the story carries you *emotionally* deeper, converging at the First Garden and the final choice.

---

## 7. Engine — **confirmed: Godot 4 (C#)**

Stylized non-photoreal art, small team, free-forever and royalty-free, light builds, painless Windows/Linux/macOS export, Steam via GodotSteam. The per-region baked lighting decision (§5) plays to Godot's strengths. Unity remains the only sane fallback if we later want its bigger ready-made-systems ecosystem.

---

## Status against Sprint 1 cards
| Card | This doc's answer |
|---|---|
| Lock central premise & player identity | §1 — the Newcomer |
| Design the final choice & endings | §2 — three endings |
| Decide combat-vs-cozy ratio | §3 — ~70/30, non-lethal |
| Decide survival mechanics scope | §4 — Tend, not Survive |
| Time-of-day design | §5 — permanent Long Noon |
| Core gameplay loop spec | §6 |
| Confirm engine | §7 — Godot 4 (C#) |

Remaining Sprint 1 cards (metaphysics bible, Region 1 lore, tone guide, region-gating matrix, repo scaffold, version control, coding standards) are documentation/implementation tasks, not decisions — ready to action next.
