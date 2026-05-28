# Quest / Beat Flow

> The critical-path objective chain per region, plus the optional deep-lore track. Objective ids map to `ULongNoonQuestSubsystem::CompleteObjective` (see `Source/TheLongNoon/Systems/LongNoonQuestSubsystem.h`). Progression is descent into truth: each region's gate is a composite (tool tier + traversal + gate-build) per the [progression matrix](progression-and-gating.md). No em dashes.

## Conventions
- Objective id format: `obj_<region>_<step>`. Lore beats are delivered through fragments ([codex-fragments.md](../lore/codex-fragments.md)) and dialogue ([dialogue-scripts.md](../lore/dialogue-scripts.md)), not exposition.
- "Gate out" = what the player must do to open the next region.
- Optional deep-lore objectives are prefixed `obj_lore_` and never block finishing.

---

## Region 1 — The Sunhollow (tutorial + vertical slice)
Surface: settle in, learn the loop. Buried: the monument, the first wrongness.

1. `obj_sunhollow_wake` — wake in the meadow, reach the village (movement).
2. `obj_sunhollow_tinker` — meet the Tinker, receive the Pruning Blade (crafting/tool intro).
3. `obj_sunhollow_gather` — gather gentle Bloom with the Forager's method (gathering).
4. `obj_sunhollow_craft` — craft your first component at the bench (crafting).
5. `obj_sunhollow_prune` — prune an overgrown patch; meet one harmless animate growth (reclamation intro).
6. `obj_sunhollow_home` — claim and decorate your plot (building + Comfort).
7. `obj_sunhollow_clear_treeline` — **gate out:** clear the treeline thicket (needs Pruning Blade + Tinker build) -> opens The Overgrowth.
- Lore beats: monument inscription seen (unreadable), the Keeper's grief, the shadow-tell observation.

## Region 2 — The Overgrowth
Surface: lush abundance. Buried: the Bloom is unnatural; swallowed homes; true-names.

1. `obj_overgrowth_forager` — find the Forager's deep camp.
2. `obj_overgrowth_still` — craft and use the Stiller on dense Bloom (still mechanic).
3. `obj_overgrowth_outpost` — place a Wayanchor, establish the first outpost (fast travel).
4. `obj_overgrowth_excavate` — clear Bloom off a swallowed home; recover a true-name fragment.
5. `obj_overgrowth_choke` — **gate out:** get past The Choke (mid-tier gear + Tinker clearing build) -> opens The Stillworks.
- Lore beats: true-name thread opens (litTier 1), swallowed-home (Lineage), Bloom-warden note.

## Region 3 — The Stillworks (the pivot)
Surface: tidy emptiness. Buried: the tally, machines of ending, readable script.

1. `obj_stillworks_enter` — descend into the works.
2. `obj_stillworks_scriptkey` — recover the script-key (raises literacy to tier 2; retroactively reveals the Sunhollow monument).
3. `obj_stillworks_hall` — find the Hall of Marks; meet the One Who Counts.
4. `obj_stillworks_count` — (optional-ish) do the count math, confirm tally = pre-Noon population (Tally thread).
5. `obj_stillworks_climbseal` — craft the Sealing Spade; gain climb/seal traversal.
6. `obj_stillworks_sealed_passage` — **gate out:** open the sealed passage (top traversal + key) -> opens The Hush.
- Lore beats: memorial dedication completes the monument line, machine plate, census ledger, "someone remembers."

## Region 4 — The Hush
Surface: uncanny stillness. Buried: memory's end; the Rememberer; full literacy.

1. `obj_hush_enter` — cross into the thinning light.
2. `obj_hush_fading` — witness the Fading Ones (atmosphere beat; the lullaby fragment).
3. `obj_hush_final_fragments` — collect the last script fragments (literacy tier 3).
4. `obj_lore_find_rememberer` — **optional, deep-lore:** locate the Rememberer's refuge (requires fragment progress). Sets `bRemembererFound`.
5. `obj_lore_rememberer_truth` — **optional:** complete the Rememberer's questline; unlocks the Third Way and literacy tier 4.
6. `obj_hush_threshold` — **gate out:** reach the Threshold (top-tier gear + key items) -> opens The First Garden.
- Lore beats: confirmation fragment, the archive, the Rememberer's dagger.

## Region 5 — The First Garden (finale)
No combat. Reverent approach, conversation, choice.

1. `obj_garden_approach` — the slow walk inward (light/lore convergence).
2. `obj_garden_meet` — meet the Gardener; hear its question.
3. `obj_garden_read_word` — read the Gardener's glyph (literacy tier 4): "the kindness I took away."
4. `obj_garden_choice` — **terminal objective:** make the choice at the Seedbed. Branches per [endings.md](../lore/endings.md):
   - Ending 1 Gift Returned, or Ending 2 Long Noon Continues (always available).
   - Ending 3 Third Way (only if `bRemembererFound` and literacy tier 4).

---

## Deep-lore track (parallel, optional)
- `obj_lore_truenames` — match the Forager's invented names to recovered true-names (collectible thread).
- `obj_lore_monument` — read the full monument inscription to the Keeper (devastating optional scene).
- `obj_lore_tally` — tell the One Who Counts what the count means.
- `obj_lore_find_rememberer` / `obj_lore_rememberer_truth` — the True Ending key (above).
None gate the main path; together they deliver the full picture and Ending 3.

## Implementation notes
- Systems call `CompleteObjective(obj_id)`; the HUD/journal listens to `OnObjectiveCompleted`.
- Gate-out objectives flip a region unlock via `ULongNoonRegionSubsystem` (tool tier + `GrantTraversal` + `MarkGateBuildComplete`).
- Literacy milestones call `ULongNoonGameInstance::SetScriptLiteracyTier`, which fires the retroactive fragment reveal.
