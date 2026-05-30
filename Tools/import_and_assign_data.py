# Import the Content/Data/*.csv files as DataTables (one per row struct) and assign
# them into ULongNoonDataSettings (Project Settings -> The Long Noon Data).
# Run via the FULL editor: -ExecutePythonScript (editor closed first).
import unreal

def log(m): unreal.log("[DATA] " + m)

tools = unreal.AssetToolsHelpers.get_asset_tools()
DATA_DIR = "C:/Users/Alu/the-long-noon/Content/Data"

# (csv base, DT asset name, struct script-name, settings property)
TABLES = [
    ("Items",         "DT_Items",         "ItemRow",         "item_table"),
    ("Tools",         "DT_Tools",         "ToolRow",         "tool_table"),
    ("Recipes",       "DT_Recipes",       "RecipeRow",       "recipe_table"),
    ("Regions",       "DT_Regions",       "RegionRow",       "region_table"),
    ("LoreFragments", "DT_LoreFragments", "LoreFragmentRow", "lore_fragment_table"),
    ("NPCs",          "DT_NPCs",          "NpcRow",          "npc_table"),
    ("DialogueLines", "DT_DialogueLines", "DialogueLineRow", "dialogue_line_table"),
    ("Builds",        "DT_Builds",        "BuildRow",        "build_table"),
]

created = {}
for (csv_base, dt_name, struct_name, _prop) in TABLES:
    pkg = "/Game/Data/" + dt_name
    struct = getattr(unreal, struct_name).static_struct()
    if unreal.EditorAssetLibrary.does_asset_exist(pkg):
        dt = unreal.load_asset(pkg)
    else:
        fac = unreal.DataTableFactory()
        fac.struct = struct
        dt = tools.create_asset(dt_name, "/Game/Data", unreal.DataTable, fac)
    csv_path = "%s/%s.csv" % (DATA_DIR, csv_base)
    with open(csv_path, "r", encoding="utf-8") as f:
        csv_str = f.read()
    problems = unreal.DataTableFunctionLibrary.fill_data_table_from_csv_string(dt, csv_str)
    unreal.EditorAssetLibrary.save_loaded_asset(dt)
    n = len(unreal.DataTableFunctionLibrary.get_data_table_row_names(dt))
    log("%s: %d rows (problems=%s)" % (dt_name, n, problems))
    created[_prop] = dt

# Assign into the DeveloperSettings and persist to DefaultGame.ini.
settings = unreal.get_default_object(unreal.LongNoonDataSettings)
for (_csv, _dt, _struct, prop) in TABLES:
    settings.set_editor_property(prop, created[prop])
settings.save_config()
log("assigned 8 tables into ULongNoonDataSettings + saved config")
log("DONE")
unreal.SystemLibrary.quit_editor()
