# Import the Kenney CC0 audio (RPG Audio SFX + Music Jingles) as SoundWave assets.
# Tests whether UE 5.7 imports .ogg directly. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal
import os

def log(m): unreal.log("[AUD] " + m)
def warn(m): unreal.log_warning("[AUD] " + m)

SRC = "C:/Users/Alu/the-long-noon/ThirdParty/source"
SETS = [
    ("kenney_rpg-audio", "/Game/ThirdParty/Audio/SFX"),
    ("kenney_music-jingles", "/Game/ThirdParty/Audio/Music"),
]

tools = unreal.AssetToolsHelpers.get_asset_tools()

for (folder, dest) in SETS:
    root = os.path.join(SRC, folder)
    oggs = []
    for dirpath, _dirs, names in os.walk(root):
        for n in names:
            if n.lower().endswith(".ogg"):
                oggs.append(os.path.join(dirpath, n))
    oggs.sort()
    log("%s: found %d ogg" % (folder, len(oggs)))

    tasks = []
    for f in oggs:
        t = unreal.AssetImportTask()
        t.set_editor_property("filename", f)
        t.set_editor_property("destination_path", dest)
        t.set_editor_property("automated", True)
        t.set_editor_property("replace_existing", True)
        t.set_editor_property("save", True)
        tasks.append(t)
    if tasks:
        tools.import_asset_tasks(tasks)
    landed = unreal.EditorAssetLibrary.list_assets(dest, recursive=True) if unreal.EditorAssetLibrary.does_directory_exist(dest) else []
    log("%s: %d assets now under %s" % (folder, len(landed), dest))

log("DONE")
unreal.SystemLibrary.quit_editor()
