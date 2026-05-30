# Import the Kenney Animated Characters 3 (CC0) rigged character as a SKELETAL mesh,
# then import its idle/run/jump animation FBX as AnimSequences against that skeleton.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[ANIMCHAR] " + m)
def warn(m): unreal.log_warning("[ANIMCHAR] " + m)

SRC = "C:/Users/Alu/the-long-noon/ThirdParty/source/kenney_animated-characters-3"
DEST = "/Game/ThirdParty/CharAnim"
tools = unreal.AssetToolsHelpers.get_asset_tools()

# 1) Skeletal mesh import (creates the mesh + skeleton + physics asset).
mesh_task = unreal.AssetImportTask()
mesh_task.set_editor_property("filename", SRC + "/Model/characterMedium.fbx")
mesh_task.set_editor_property("destination_path", DEST)
mesh_task.set_editor_property("automated", True)
mesh_task.set_editor_property("replace_existing", True)
mesh_task.set_editor_property("save", True)
mopts = unreal.FbxImportUI()
mopts.set_editor_property("import_mesh", True)
mopts.set_editor_property("import_as_skeletal", True)
mopts.set_editor_property("import_materials", True)
mopts.set_editor_property("import_textures", True)
mopts.set_editor_property("import_animations", False)
mopts.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_SKELETAL_MESH)
mesh_task.set_editor_property("options", mopts)
tools.import_asset_tasks([mesh_task])
log("imported skeletal mesh")

# Find the skeleton asset that the mesh import created.
skeleton = None
for path in unreal.EditorAssetLibrary.list_assets(DEST, recursive=True):
    a = unreal.load_asset(path)
    if isinstance(a, unreal.Skeleton):
        skeleton = a
        log("skeleton = " + path)
        break
if not skeleton:
    warn("no skeleton found; cannot import animations")
    unreal.SystemLibrary.quit_editor()
    raise SystemExit

# 2) Animation imports against that skeleton.
anim_tasks = []
for anim in ["idle", "run", "jump"]:
    t = unreal.AssetImportTask()
    t.set_editor_property("filename", "%s/Animations/%s.fbx" % (SRC, anim))
    t.set_editor_property("destination_path", DEST)
    t.set_editor_property("automated", True)
    t.set_editor_property("replace_existing", True)
    t.set_editor_property("save", True)
    aopts = unreal.FbxImportUI()
    aopts.set_editor_property("import_mesh", False)
    aopts.set_editor_property("import_as_skeletal", True)
    aopts.set_editor_property("import_animations", True)
    aopts.set_editor_property("import_materials", False)
    aopts.set_editor_property("mesh_type_to_import", unreal.FBXImportType.FBXIT_ANIMATION)
    aopts.set_editor_property("skeleton", skeleton)
    t.set_editor_property("options", aopts)
    anim_tasks.append(t)
tools.import_asset_tasks(anim_tasks)
log("imported %d animations" % len(anim_tasks))

for path in unreal.EditorAssetLibrary.list_assets(DEST, recursive=True):
    log("asset: " + path)
log("DONE")
unreal.SystemLibrary.quit_editor()
