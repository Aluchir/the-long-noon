# Import the Kenney Nature Kit (CC0) FBX models into /Game/ThirdParty/NatureKit with
# their palette materials. Run via FULL editor (-ExecutePythonScript), editor closed
# first. The models use flat per-material diffuse colors (leafsDark, woodBirch, ...),
# so import_materials carries the look; no textures needed.
import unreal
import os

def log(m): unreal.log("[NK] " + m)
def warn(m): unreal.log_warning("[NK] " + m)

SRC = "C:/Users/Alu/the-long-noon/ThirdParty/source/kenney_nature-kit/Models/FBX format"
DEST = "/Game/ThirdParty/NatureKit"

files = [os.path.join(SRC, f) for f in os.listdir(SRC) if f.lower().endswith(".fbx")]
files.sort()
log("found %d FBX" % len(files))

tasks = []
for fbx in files:
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", fbx)
    task.set_editor_property("destination_path", DEST)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)

    opts = unreal.FbxImportUI()
    opts.set_editor_property("import_mesh", True)
    opts.set_editor_property("import_as_skeletal", False)
    opts.set_editor_property("import_materials", True)
    opts.set_editor_property("import_textures", False)
    opts.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
    sm = opts.static_mesh_import_data
    sm.set_editor_property("combine_meshes", True)
    sm.set_editor_property("generate_lightmap_u_vs", True)
    sm.set_editor_property("auto_generate_collision", True)
    task.set_editor_property("options", opts)
    tasks.append(task)

unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)

# Count what landed.
imported = unreal.EditorAssetLibrary.list_assets(DEST, recursive=True)
meshes = [a for a in imported if ".uasset" not in a]  # list_assets returns object paths
log("DEST now holds %d asset entries" % len(imported))
log("DONE")
unreal.SystemLibrary.quit_editor()
