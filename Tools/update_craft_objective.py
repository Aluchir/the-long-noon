# Make the Sunhollow opening loop completable: reimport DT_Recipes (so warm_broth now
# completes 'craft_broth') and retarget the live quest board's craft step from
# 'craft_blade' (uncraftable here) to 'craft_broth' (craftable from gathered sunmoss).
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[CRAFTOBJ] " + m)
def warn(m): unreal.log_warning("[CRAFTOBJ] " + m)

# 1) Reimport the Recipes DataTable from the edited CSV.
dt = unreal.load_asset("/Game/Data/DT_Recipes")
if dt:
    with open("C:/Users/Alu/the-long-noon/Content/Data/Recipes.csv", "r", encoding="utf-8") as f:
        csv_str = f.read()
    unreal.DataTableFunctionLibrary.fill_data_table_from_csv_string(dt, csv_str)
    unreal.EditorAssetLibrary.save_loaded_asset(dt)
    log("DT_Recipes reimported")
else:
    warn("DT_Recipes not found")

# 2) Update the Sunhollow quest board objectives.
les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
les.load_level("/Game/Maps/L_Sunhollow_Greybox")
updated = 0
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "QuestBoard":
        try:
            a.set_editor_property("objectives", ["gather_sunmoss", "craft_broth", "prune_bloom", "build_gate"])
            updated += 1
        except Exception as e:
            warn("set objectives failed: %s" % e)
les.save_current_level()
log("updated %d quest board(s), saved" % updated)
log("DONE")
unreal.SystemLibrary.quit_editor()
