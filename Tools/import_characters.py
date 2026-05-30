# Import the Kenney Mini Characters (CC0) humanoid FBX as static meshes into
# /Game/ThirdParty/Characters (bind pose; fine for standing NPCs). Run via FULL
# editor (-ExecutePythonScript), editor closed first.
import unreal
import os

def log(m): unreal.log("[CHAR] " + m)

SRC = "C:/Users/Alu/the-long-noon/ThirdParty/source/kenney_mini-characters"
DEST = "/Game/ThirdParty/Characters"

files = []
for dirpath, _dirs, names in os.walk(SRC):
    for n in names:
        if n.lower().startswith("character-") and n.lower().endswith(".fbx"):
            files.append(os.path.join(dirpath, n))
files.sort()
log("found %d character FBX" % len(files))

tasks = []
for fbx in files:
    t = unreal.AssetImportTask()
    t.set_editor_property("filename", fbx)
    t.set_editor_property("destination_path", DEST)
    t.set_editor_property("automated", True)
    t.set_editor_property("replace_existing", True)
    t.set_editor_property("save", True)
    opts = unreal.FbxImportUI()
    opts.set_editor_property("import_mesh", True)
    opts.set_editor_property("import_as_skeletal", False)
    opts.set_editor_property("import_materials", True)
    opts.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
    opts.static_mesh_import_data.set_editor_property("combine_meshes", True)
    opts.static_mesh_import_data.set_editor_property("auto_generate_collision", True)
    t.set_editor_property("options", opts)
    tasks.append(t)
unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)

landed = unreal.EditorAssetLibrary.list_assets(DEST, recursive=True)
log("DEST now holds %d entries" % len(landed))
log("DONE")
unreal.SystemLibrary.quit_editor()
