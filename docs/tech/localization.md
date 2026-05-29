# Localization Scaffolding

> How The Long Noon stays translatable. Set up now (cheap), pay off later. The headline scope win: **no voice acting** (NPCs use [wordless vocalization](../audio/audio-direction.md#6-voice--vocalization)), so localization is **text-only** — no dubbing, no lip-sync, no localized audio. This doc is the plan and the in-editor checklist; it is authored, not yet executed (needs the UE editor).

## 1. The one rule that makes everything translatable
**Every player-facing string is an `FText`.** The codebase already follows this:
- Data: `FItemRow`/`UItemDef`, `FToolRow`, `FLoreFragmentRow`, `FDialogueLineRow` use `FText` for `DisplayName`, `FlavorText`, `Text`, `GlyphText`, `TranslatedText`.
- Code/UI: HUD prompts, dialogue broadcasts, codex text are all `FText`.
- Never build display strings with `FString` + concatenation. Use `FText::Format` with named args so word order can change per language.

## 2. The forgotten-script layer is orthogonal to i18n
Two different "translations" exist; do not conflate them:
- **In-fiction cipher** (`GlyphText` → `TranslatedText`, gated by `RequiredLiteracyTier`): a *gameplay* reveal. The [cipher font](../art/forgotten-script-font-spec.md) renders Latin plain text as botanical glyphs.
- **Real localization** (English → French/…): a *build* concern handled by UE's localization pipeline.

They compose cleanly: in a French build, `GlyphText` holds French plain text and the cipher font still renders meaningful glyphs (the font is language-agnostic over Latin + PUA logograms). So **inscriptions localize for free** as long as they stay font-rendered text and are never baked into textures. This is a hard rule: **no baked-in text on any art asset** (see [asset list](../art/asset-list.md)).

## 3. Localization targets & sources
One UE localization **target: `Game`**. Gather text from:
- **C++** — `LOCTEXT`/`NSLOCTEXT` (define `LOCTEXT_NAMESPACE` per file).
- **Blueprints** — pin literals authored as text.
- **DataTables** — the CSV-imported tables (`Items`, `Tools`, `LoreFragments`, `NPCs`, `DialogueLines`, `Regions`). UE's gather reads `FText` columns; keep the CSV the source of truth and re-import on change (see [data authoring guide](data-authoring-guide.md)).
- **String Table(s)** — for standalone UI strings not living in data; author as `Content/Localization/UIStrings` and reference by key from UMG.

## 4. Cultures
- **Source culture:** `en`. Ship English first.
- **Planned:** EFIGS (`fr`, `it`, `de`, `es`), then `pt-BR`, `ja`, `ko`, `zh-Hans` as reach allows. No audio localization needed (wordless VO), so each language is text + QA only.
- **`de` is the layout stress test** — German strings run long; the [HUD/UI](../art/style-guide.md#8-ui-art-direction) must wrap/scale (already a stated UI requirement). Build UI with auto-sizing panels, never fixed-width text boxes.

## 5. Workflow (in-editor, per pass)
1. Window → Localization Dashboard → create target `Game`.
2. Add gather steps: **Gather Text from Source Code**, **from Packages** (Blueprints/DataTables), **from Metadata** as needed.
3. Add cultures (§4). Enable **pseudolocalization** (`xx-LOCM`) early to catch non-`FText` strings and clipping before any real translation exists.
4. Gather → export `.po` per culture → translate → import → **compile to `.locres`**.
5. Output lands in `Content/Localization/Game/<culture>/Game.locres`; config in `Config/DefaultGame.ini` (localization paths) and `Config/Localization/*.ini`.
6. Set runtime language from a settings menu via `FInternationalization`/`SetCurrentCulture`; default to OS culture with English fallback.

## 6. Authoring rules for translatable text
- Author with `FText::Format(NSLOCTEXT(...), Args)`; never concatenate fragments or embed numbers/order assumptions.
- Give each string a **stable key**; don't reuse one key for two meanings (homographs differ across languages).
- Keep house style ([no em dashes](../lore/tone-and-writing-style-guide.md)) in the **source**; translators inherit tone notes.
- Mark genuinely non-translatable tokens (ids, the logogram tags like `[KINDNESS]`) as **`InvariantText`/culture-invariant** so they are not sent to translators.
- Plurals/gender: use ICU message format inside the `FText` so translators control plural forms.

## 7. What is NOT localized
- Internal ids (`tool_pruning_blade`, `frag_*`, `npc_*`), DataTable RowNames, enum tokens, recipe/verb encodings.
- Audio (wordless VO + music + SFX are universal).
- The cipher *glyphs* themselves (font-rendered; the underlying plain text is what localizes).

## 8. Checklist (do once UE is installed)
- [ ] Confirm every new player-facing string is `FText` (grep for display `FString`s).
- [ ] Create the `Game` localization target + source culture `en`.
- [ ] Wire DataTable + String Table gather steps.
- [ ] Turn on pseudolocalization; fix every clipped/hardcoded string it surfaces.
- [ ] Stand up a `de` pass purely to validate UI layout headroom.
- [ ] Document the language-select menu hook in the settings UI.
