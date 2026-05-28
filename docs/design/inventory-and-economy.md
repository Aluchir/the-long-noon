# Inventory & Economy Design

> How the player holds, stores, and spends. The economy is a **closed material loop** (gather → craft → build/upgrade), not a money market — there is no currency, because a deathless, post-scarcity world has little use for one. Trade is barter-light and social, not transactional.

## 1. Goals
- **Cozy, low-stress inventory** — generous, organized, never a tedious management mini-game.
- **Closed material economy** — materials are the only "currency"; everything is crafted, not bought.
- **Storage that respects the loop** — easy deposit/withdraw, craft-from-storage, outpost storage at the frontier.

## 2. Inventory model
- **Personal inventory:** slot- or weight-light system; generous base capacity, expandable via crafted bags/upgrades.
- **Stacking:** materials stack high; tools/unique items don't.
- **Categories/auto-sort:** materials, tools, components, consumables, decor, lore/key items (key + lore items in a separate, non-droppable tab).
- **No hard overflow punishment:** if full, excess gather goes to a "satchel overflow" the player reconciles at storage — never lost.

## 3. Storage
- **Home & outpost storage chests** — shared/linked storage so the player isn't shuttling materials.
- **Craft-from-storage:** stations pull from linked storage automatically (a major cozy-QoL standard).
- **Auto-deposit** option for raw materials at a station.

## 4. The economy — a closed material loop
- **No money.** No gold, no shop currency. The world is deathless and post-scarcity; hoarding wealth is meaningless here (a quiet thematic point).
- **Materials are the economy:** tiered, region-sourced (see [resource economy](resource-economy.md)); spent on [crafting](crafting.md) and [building](building.md).
- **Sinks:** tool crafting/upgrades, building components, gate-builds, consumables, repair kits, decor.
- **Sources:** Bloom harvesting, ruin excavation, pruning byproducts, region-specific nodes.
- **Balance principle:** each tier's sinks are tuned to that tier's sources so progression paces naturally (gather a region → afford its tools → move on).

## 5. Trade & NPCs (barter-light)
- NPCs **give and request** rather than buy/sell. The Tinker teaches recipes; the Forager exchanges field-guide knowledge for samples; requests yield recipes, decor, or lore — not money.
- Trade is framed **socially** ("bring me three morningspit and I'll show you something") — warm, not transactional, fitting the world.
- This keeps the economy closed and the tone cozy, and avoids a currency that would feel out of place.

## 6. Consumables & upkeep
- **Food:** restores Stamina/Focus, raises Comfort; crafted from gathered ingredients.
- **Repair kits:** restore tool durability (the main upkeep pressure).
- All consumables are craftable from abundant materials — upkeep is a gentle rhythm, never a crisis.

## 7. UX
- Clean inventory grid with auto-sort, search/filter, and a separate locked tab for key/lore items.
- One-button deposit-all / restock-from-storage.
- Clear "you have / you need" feedback when crafting or building.
- Material tooltips carry [lore flavor](../lore/weapon-and-tool-lore.md) (literacy-aware).

## 8. Implementation notes
- Items: `{id, category, stackMax, droppable, flavor}`.
- Storage as linked containers; stations query a storage graph for craft-from-storage.
- Economy tuning in a shared balance sheet with [crafting](crafting.md) and [resource economy](resource-economy.md): per-tier source rates vs sink costs.
- No currency entity — barter is recipe/quest-driven exchange, modeled as request→reward, not price.
