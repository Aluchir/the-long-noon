# Build L_MainMenu: a near-empty title-screen level whose GameMode override is the
# C++ ALongNoonMenuGameMode (which shows the main-menu widget on BeginPlay). Also set
# the project's default maps to this level so packaged builds boot to the title screen.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[MENU] " + m)
def warn(m): unreal.log_warning("[MENU] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_MainMenu"
if unreal.EditorAssetLibrary.does_asset_exist(map_pkg):
    unreal.EditorAssetLibrary.delete_asset(map_pkg)
    log("deleted stale level asset")
les.new_level(map_pkg)
log("new level")

# A PlayerStart so a default pawn/controller exists for the menu input.
eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, 0.0, 100.0)).set_actor_label("PlayerStart")

# GameMode override = the C++ menu GameMode (shows the title widget on BeginPlay).
try:
    gm = unreal.LongNoonMenuGameMode
    ues.get_editor_world().get_world_settings().set_editor_property("default_game_mode", gm)
    log("menu gamemode override set")
except Exception as e:
    warn("gamemode override failed: %s" % e)

les.save_current_level()
log("saved " + map_pkg)

# Point the project's default maps at the menu so cooked/standalone boots to it.
try:
    gms = unreal.get_default_object(unreal.GameMapsSettings)
    gms.set_editor_property("game_default_map", unreal.SoftObjectPath(map_pkg))
    gms.set_editor_property("editor_startup_map", unreal.SoftObjectPath(map_pkg))
    gms.save_config()
    log("project default maps -> L_MainMenu")
except Exception as e:
    warn("default-map set failed: %s" % e)

log("DONE")
unreal.SystemLibrary.quit_editor()
