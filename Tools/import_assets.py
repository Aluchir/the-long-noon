# Asset import pipeline for free/CC0 packs.
# Drop raw asset files (FBX/glTF/OBJ meshes, PNG/JPG textures, WAV/OGG audio) into
# the repo's AssetDrop/ folder, then run this via the FULL editor (-ExecutePythonScript)
# to import them all into /Game/ThirdParty (mirroring the drop folder structure).
# Fab/Marketplace packs that arrive as .uasset need no import - this is for raw files.
import unreal
import os

def log(m): unreal.log("[IMPORT] " + m)
def warn(m): unreal.log_warning("[IMPORT] " + m)

DROP = "C:/Users/Alu/the-long-noon/AssetDrop"
DEST_ROOT = "/Game/ThirdParty"
MESH_EXT = (".fbx", ".gltf", ".glb", ".obj")
TEX_EXT = (".png", ".jpg", ".jpeg", ".tga", ".bmp")
SND_EXT = (".wav", ".ogg")

if not os.path.isdir(DROP):
    warn("no AssetDrop folder at %s - create it and drop pack files in." % DROP)
    unreal.SystemLibrary.quit_editor()
else:
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    tasks = []
    counts = {"mesh": 0, "tex": 0, "snd": 0, "skip": 0}
    for root, _dirs, files in os.walk(DROP):
        rel = os.path.relpath(root, DROP).replace("\\", "/")
        dest = DEST_ROOT if rel == "." else DEST_ROOT + "/" + rel
        for f in files:
            ext = os.path.splitext(f)[1].lower()
            if ext in MESH_EXT: counts["mesh"] += 1
            elif ext in TEX_EXT: counts["tex"] += 1
            elif ext in SND_EXT: counts["snd"] += 1
            else:
                counts["skip"] += 1
                continue
            t = unreal.AssetImportTask()
            t.filename = os.path.join(root, f)
            t.destination_path = dest
            t.automated = True
            t.replace_existing = True
            t.save = True
            tasks.append(t)
    if tasks:
        tools.import_asset_tasks(tasks)
    log("imported tasks=%d (meshes=%d textures=%d sounds=%d, skipped=%d)" %
        (len(tasks), counts["mesh"], counts["tex"], counts["snd"], counts["skip"]))
    log("DONE")
    unreal.SystemLibrary.quit_editor()
