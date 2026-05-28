# Weapon & Tool Lore (flavor text)

> Every reclamation tool is non-lethal (you prune, still, and subdue the Bloom — you never kill). And every tool is a **puzzle piece**: its flavor text quietly carries lore. Players who read their gear assemble part of the story. Tiers gate regions (see [progression matrix](../design/progression-and-gating.md)).

## The principle
Tools look like weapons but are framed as **gardening implements for a world that grows too much.** Their flavor text uses the [forgotten script](forgotten-script.md) and reclaimed materials to drip-feed truth — a label no one can read, a part salvaged from a machine built for endings, a metal that remembers being something else.

Flavor escalates with tier: tier 1 reads cozy and practical; by tier 4 the descriptions are openly haunted, matching the player's growing understanding.

---

## Reclamation tools (the "weapons")

### Tier 1 — The Pruning Blade *(Sunhollow)*
> A simple curved blade for cutting back the gentle Bloom. The Tinker made you one without being asked, the way you'd hand a guest a cup of tea. *"Everyone should be able to tend their own patch,"* they said, and meant it kindly, and forgot they'd said it by evening.

### Tier 2 — The Stiller *(Overgrowth)*
> Hum it against the Bloom and a patch goes briefly dormant, dreaming. The Forager has three names for what it does and isn't sure any are right. None of them is "kill." The world has no word for that anymore.

### Tier 2 — The Resonance Hammer *(Overgrowth)*
> For the animate growths that won't be reasoned with. It doesn't break them; it *settles* them, sends them back to sleep. The head is cast from a swallowed-home's doorframe. Someone lived behind that door. You'll never know who. Neither will they.

### Tier 3 — The Sealing Spade *(Stillworks)*
> Half tool, half key. Cuts, climbs, and seals the managed Bloom of the old works. Its haft bears a maker's mark in the forgotten script. When you can finally read it, it says only a name, and a date that can't be right, and the word the world lost for *"in memory of."*

### Tier 3 — The Quieting Edge *(Stillworks)*
> Forged from a Stillworks part labeled in a script no one can read. The label, once you can, names the machine's purpose: it was built to help things *end* gently. The edge remembers a kinder job than tending overgrowth. So, in its way, does the whole world.

### Tier 4 — The Long Tool *(The Hush)*
> In the thin bronze light of the Hush even the Bloom forgets to grow, and this tool barely needs to work. It is the finest thing you will make and you will use it least. The Rememberer looked at it for a long time and said it reminded them of something, and then, for once, *did* remember, and wished they hadn't.

### Tier 5 — *(no new tool — the First Garden takes no pruning)*
> You bring your whole kit to the First Garden and find nothing to cut. The light is perfect. Everything is already still. There was never anything here to subdue. Only a question, and a gardener, waiting to be told whether it did the kind thing.

---

## Utility tools (gathering / traversal / building)
- **The Field Hook** *(R1)* — for gathering gentle Bloom. *"Take only what you'll use,"* the Forager says, and then can't recall why that mattered, only that it did.
- **The Climb-Iron** *(R3)* — for the old vertical works. Its grip is worn smooth by hands that climbed here long before yours and are, somewhere, climbing still, having forgotten they ever stopped.
- **The Wayanchor** *(R2)* — establishes an outpost / fast-travel point. *"A place you can always come back to,"* the Tinker explains, brightly, in a world where no one remembers leaving.
- **The Builder's Kit** *(R1)* — claim and shape your home plot. The first thing the Tinker teaches, and the thing they have, themselves, done more times than counting.

---

## Material flavor (the resource layer carries lore too)
- **Sunmoss** *(R1)* — soft, warm, endlessly renewing. The kindest material in the world, and the most thoughtless.
- **Swallowed-wood** *(R2)* — timber reclaimed from homes the Bloom ate. Still faintly shaped by the rooms it used to be.
- **Stillsteel** *(R3)* — salvage from the dormant machines. It holds an edge and, the smiths swear, a mood.
- **Hushglass** *(R4)* — formed where the light went thin and slow. Looks like a held breath. Almost no one comes this deep to gather it.
- **Gardenlight** *(R5)* — not gathered. Given, or not, depending on how the story ends.

## Implementation notes
- Store flavor as `{itemId, tier, region, flavorGlyphs?, flavorTranslation, requiredTier}` so script-bearing flavor (the maker's marks, machine labels) reveals with [literacy](forgotten-script.md), exactly like codex entries.
- Tier → region gating defined in the [progression matrix](../design/progression-and-gating.md); tiers map 1:1 to the five regions' Bloom densities.
