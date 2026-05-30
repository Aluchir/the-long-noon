# Build a plain (non-World-Partition) test level: floor, light, sky, player start,
# GameMode override. Run via the FULL editor (-ExecutePythonScript), where the
# LevelEditorSubsystem is available (it is null in the -run=pythonscript commandlet).

import unreal

def log(m): unreal.log("[FLAT] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_TestFlat"
les.new_level(map_pkg)
log("new level created")

# Floor: engine cube scaled into a wide slab, top surface near Z=0.
cube = unreal.load_asset("/Engine/BasicShapes/Cube")
floor = eas.spawn_actor_from_object(cube, unreal.Vector(0.0, 0.0, -50.0))
floor.set_actor_scale3d(unreal.Vector(50.0, 50.0, 1.0))
floor.set_actor_label("Floor")

dl = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 1000.0), unreal.Rotator(-45.0, 0.0, 0.0))
dl.set_actor_label("Sun")
sky = eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0.0, 0.0, 600.0))
sky.set_actor_label("Sky")
atmo = eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0))
atmo.set_actor_label("SkyAtmosphere")
ps = eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, 0.0, 150.0))
ps.set_actor_label("PlayerStart")
log("actors spawned")

# Force our GameMode on this level.
gm = unreal.load_class(None, "/Game/GameModes/BP_LongNoonGameMode.BP_LongNoonGameMode_C")
try:
    world = ues.get_editor_world()
    world.get_world_settings().set_editor_property("default_game_mode", gm)
    log("gamemode override set")
except Exception as e:
    unreal.log_warning("[FLAT] gamemode override failed: %s" % e)

les.save_current_level()
log("DONE saved " + map_pkg)
unreal.SystemLibrary.quit_editor()
