# The Endings

> The three resolutions at the First Garden, scripted. The whole game walks toward the Gardener's question; the player answers it. The game never says which answer is right. Tone per the [style guide](tone-and-writing-style-guide.md): no villain, no cruelty, the ache lives in the choice. No em dashes.

## The setup (shared)
The Newcomer reaches the Gardener at the heart of the Long Noon. It is gentle, anxious, proud. It asks whether it did right, and then it makes its offer: it can undo what it did, if only the player will tell it what the world would want. The choice is presented at the Seedbed. There is no timer, no combat, no fail state. Only silence, and a decision.

Gating: endings 1 and 2 are always available. Ending 3 unlocks only if the player found the **Rememberer** in the Hush and completed enough of the lore (script literacy tier 4). See `ULongNoonGameInstance::bRemembererFound` and `ScriptLiteracyTier`.

---

## Ending 1 — The Gift Returned (death restored)
*The player asks the Gardener to undo its mercy.*

**Beat:** the player chooses to give it back. The Gardener does not argue. It is almost relieved to be told.
- Gardener: "All of it? The ending, and the grief that comes with the ending? The losing of each other?"
- Gardener: "Then I will give back the weight. I am sorry I could not tell the difference. I only ever wanted you to stop hurting."

**Epilogue:** the sun moves. For the first time in an age, the Long Noon tips toward evening, and a real night comes, and it is frightening and beautiful and means something. People begin to age. Somewhere, eventually, someone is born. The Keeper finally knows who the monument is for and can grieve them properly. It will hurt now. It will all hurt now. It will also, finally, matter. **Bittersweet. Not framed as right or wrong.**

---

## Ending 2 — The Long Noon Continues (stasis kept)
*The player chooses to leave the world painless and hollow.*

**Beat:** the player cannot do it. Or will not. The cost of returning death is too cruel to choose for everyone.
- Gardener: "You will keep my kindness, then. Thank you. I think. You do not look certain. Neither am I, anymore."

**Epilogue:** the light does not change. The Sunhollow stays warm and lovely forever. Everyone stays kind, and incurious, and untethered, and they will never know what they lost, because the one mind that could have told them, the Newcomer, now carries the whole truth alone. The final shot is the player standing in the golden meadow that started it all, the only one in the world who remembers, and who chose. **Quiet, lonely, unresolved on purpose.** (Mirrors the Rememberer's burden, now the player's.)

---

## Ending 3 — The Third Way (memory returned)
*Unlocked via the Rememberer. The player refuses both the lie and the blunt undoing.*

**Beat:** the player proposes a third thing the Gardener never imagined: do not force death back on everyone, and do not preserve the forgetting. Give the world back its **memory**, and let each person choose, one at a time, whether to remember, and so whether to become mortal again.
- Rememberer (present): "Not for them, all at once. To them. Let them choose what you and I never got to."
- Gardener: "Choose. I did not think to let them choose. I thought kindness was a thing you did to someone. You are telling me it is a thing you offer."

**Epilogue:** memory comes back, gently, like a tide. People begin to remember. Some choose to keep remembering, and accept that they will one day end, and weep, and hold each other differently. Some are not ready, and that is allowed. The Rememberer is, for the first time in an age, not alone in remembering. The Newcomer is no longer the only one who feels. The light is still golden, but now it is a choice to stand in it. **The most hopeful ending, and the hardest to reach.**

---

## Design notes
- All three get a distinct musical variation of the main theme returning whole (mortal/bittersweet, frozen/lonely, hopeful/shared) per the [audio direction](../audio/audio-direction.md).
- No ending is the "good" ending in dialogue or UI. Achievements/credits treat them as equal.
- The choice is recorded for a New Game Plus framing later (optional): replays can carry the knowledge, never the comfort.
- Quest hooks: the choice completes the terminal objective in the [quest flow](../design/quest-flow.md); ending 3 additionally requires the Rememberer objective chain.
