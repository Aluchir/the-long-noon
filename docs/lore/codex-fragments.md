# Codex Fragments — full text

> The actual fragment content the player collects, expanding the [catalogue](codex-catalogue.md) into authored entries. Each maps 1:1 to a `ULoreFragmentDef` row (see `Source/TheLongNoon/Data/LoreFragmentDef.h`) and seeds `Content/Data/LoreFragments.csv`. Fields: id, type, thread, region tier, required literacy tier, and the text (glyph text shows below the tier, translated text at/above it). Tone per the [style guide](tone-and-writing-style-guide.md). No em dashes.

> Readability: Observation/Artifact/Testimony fragments are the Newcomer's own words or plain objects (always readable). Inscription/TrueName fragments carry a `RequiredLiteracyTier`; below it the player sees only glyphs (see [cipher](forgotten-script-cipher.md)).

---

## Region 1 — The Sunhollow (tier 1)

**frag_sunhollow_monument** · Inscription · thread: Monument · litTier 2
- Glyphs (below tier 2): "a face of worn stone, its carving soft as moss, saying nothing you can read."
- Translated (tier 2+): "IN MEMORY OF THOSE WHO" (the line breaks; the rest waits in the Stillworks).

**frag_shadow_wrong** · Observation · thread: Rememberer · litTier 0
- "My shadow falls a little long, a little warm at the edge, like it remembers a different sun. No one else's does. I have started watching for one that matches mine. There isn't one."

**frag_no_one_asks** · Observation · thread: (none) · litTier 0
- "I have been here days and no one has asked where I came from. They are so kind. They are not curious at all. I cannot tell yet if that is peace or something missing."

**frag_tinker_kept_tool** · Artifact · thread: (none) · litTier 0
- "A small worn tool in the Tinker's locked drawer. Older than anything else here. They never use it and cannot bring themselves to scrap it, and do not know why. The only thing in the Sunhollow that feels older than memory."

---

## Region 2 — The Overgrowth (tier 2)

**frag_morningspit_truename** · TrueName · thread: TrueNames · litTier 1
- Glyphs (below tier 1): "a name carved into a root, in the old growing-script."
- Translated (tier 1+): "[the plant the Forager calls morningspit] true name: dawnweep. A better name. Older. She has never read it."

**frag_swallowed_home** · Artifact · thread: Lineage · litTier 1
- "The Bloom ate a house here. Through the green you can make out a doorway, a hearth, two chairs set close. People lived behind that door, and the room was made for more than one kind of closeness. There is a word for that the world has lost."

**frag_bloom_warden_note** · Inscription · thread: Monument · litTier 1
- Glyphs (below tier 1): "a tender's note pinned under glass, growing-script faded."
- Translated (tier 1+): "the Bloom does not stop because nothing stops. Cut it kindly. It is only doing the one thing left to it, which is to keep going."

---

## Region 3 — The Stillworks (tier 3)

**frag_memorial_dedication** · Inscription · thread: Monument · litTier 2
- Glyphs (below tier 2): "the great monument's dedication, in formal old script."
- Translated (tier 2+): "IN MEMORY OF THOSE WHO ENDED, THAT WE WHO REMAIN MIGHT REMEMBER THEM." (completes the Sunhollow line.)

**frag_census_ledger** · Artifact · thread: Tally · litTier 2
- Glyphs (below tier 2): "a brittle ledger, columns of the old counting-glyphs."
- Translated (tier 2+): "the last full count before the day the sun stopped. The number is very large. It is the same number, you will realize, that the One Who Counts is still scoring into the wall, one mark at a time, having forgotten it is a number of people."

**frag_machine_plate** · Inscription · thread: Machines · litTier 2
- Glyphs (below tier 2): "a maker's plate on a dormant machine."
- Translated (tier 2+): "this engine tended the ENDING: it received what had finished, and returned it gently to the world. It has had nothing to receive in a very long time."

**frag_someone_remembers** · Observation · thread: Rememberer · litTier 0
- "Three people now have said the same odd thing without meaning to: that there is one who remembers. They cannot say who, or where. They say it the way you'd mention a draft in a room. Then they forget they said it."

---

## Region 4 — The Hush (tier 4)

**frag_last_fragments** · Inscription · thread: Machines · litTier 3
- Glyphs (below tier 3): "the deepest, oldest writing in the world, nearly whole."
- Translated (tier 3+): "we asked the Kind One to take the suffering, and it heard us, and it took the ending, because to the Kind One they were the same. We did not have the words to tell it they were not. We are only now finding the words."

**frag_remembrancer_archive** · Artifact · thread: Rememberer · litTier 3
- "The Rememberer's shelves. The only intact history left, written in a hand that never faltered. To read it all is to hold the whole lost world for a moment, and to understand why one person, holding it alone forever, would choose to hide."

**frag_fading_lullaby** · Testimony · thread: Lineage · litTier 0
- "One of the Fading Ones hums half of something, over and over, the second half gone. It is a lullaby. Lullabies were sung to children. There are no children. There have not been for an age. They hum it anyway, to no one, having forgotten who it was for."

**frag_confirmation** · Inscription · thread: Gardener · litTier 3
- Glyphs (below tier 3): "a final account, scratched late and shaking."
- Translated (tier 3+): "it is still here. It still tends us. It is waiting, I think, to be told whether it did right. Someone should go and answer it. Someone new, who can still feel the weight of the question."

---

## Region 5 — The First Garden (tier 5)

**frag_gardeners_word** · Inscription · thread: Gardener · litTier 4
- Glyphs (below tier 4): "a single luminous glyph, repeated softly across the whole garden."
- Translated (tier 4+): "the Gardener's own word for what it removed. It has no other. As near as you can hear it: the kindness I took away."

**frag_is_everyone_happy** · Testimony · thread: Gardener · litTier 0
- "It asked me, the moment it saw me: is everyone happy now? It has been waiting an age to ask. It is so gentle. It is so sure it was kind. There is no one here to be angry at, and that is the worst of it."

**frag_lights_source** · Observation · thread: (none) · litTier 0
- "The golden light I have walked under since the first meadow comes from here. It always did. The warmth of the whole world was leaking out of this one held breath. I came all this way toward the kindest thing, and found the catastrophe wearing its face."

---

## Authoring notes
- IDs are stable and become the `FragmentId` keys in `Content/Data/LoreFragments.csv`.
- "Translated" text is the `TranslatedText`; "Glyphs" descriptions are placeholder `GlyphText` (the font renders real cipher glyphs, see [cipher](forgotten-script-cipher.md)).
- Threads (Monument, Tally, Machines, TrueNames, Lineage, Gardener, Rememberer) drive the codex's thread view.
- The retroactive reveal of **frag_sunhollow_monument** at literacy tier 2 is the marquee "you can now read it" moment; pair it with the Keeper scene.
