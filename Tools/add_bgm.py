# Add a looping ambient BGM bed to each region: an AmbientSound playing a region-
# appropriate Kenney CC0 Music Jingle (set to loop). Loads each existing map (preserves
# decor/lights) and re-saves. Idempotent (clears prior "BGM" actor). Run via FULL
# editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[BGM] " + m)
def warn(m): unreal.log_warning("[BGM] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
MUS = "/Game/ThirdParty/Audio/Music/"

# One track per region (calm pizzicato/sax families). Set to loop as a gentle bed.
REGIONS = {
    "L_Sunhollow_Greybox":   "jingles_PIZZI01",
    "L_Overgrowth_Greybox":  "jingles_PIZZI02",
    "L_Stillworks_Greybox":  "jingles_SAX01",
    "L_Hush_Greybox":        "jingles_PIZZI03",
    "L_FirstGarden_Greybox": "jingles_SAX02",
}

def add_bgm(map_pkg, track):
    les.load_level("/Game/Maps/" + map_pkg)
    wave = unreal.load_asset(MUS + track)
    if not isinstance(wave, unreal.SoundWave):
        warn("%s: track '%s' not a SoundWave" % (map_pkg, track)); return
    # Loop the bed.
    try:
        wave.set_editor_property("looping", True)
        unreal.EditorAssetLibrary.save_loaded_asset(wave)
    except Exception as e:
        warn("looping set failed: %s" % e)

    removed = 0
    for a in eas.get_all_level_actors():
        if a.get_actor_label() == "BGM":
            eas.destroy_actor(a); removed += 1

    amb = eas.spawn_actor_from_class(unreal.AmbientSound, unreal.Vector(0.0, 0.0, 200.0))
    amb.set_actor_label("BGM")
    comp = amb.get_component_by_class(unreal.AudioComponent)
    if comp:
        comp.set_sound(wave)
        comp.set_editor_property("auto_activate", True)
        comp.set_editor_property("volume_multiplier", 0.55)
    les.save_current_level()
    log("%s: BGM=%s (removed %d old), saved" % (map_pkg, track, removed))

for map_pkg, track in REGIONS.items():
    add_bgm(map_pkg, track)

log("ALL BGM ADDED")
unreal.SystemLibrary.quit_editor()
