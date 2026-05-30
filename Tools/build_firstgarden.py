# Build L_FirstGarden_Greybox: a plain (non-WP) stub of Region 5 (the First Garden),
# the climax. Holds the Gardener NPC and the three ending choice markers (the Third
# Way only resolves if the Rememberer was found). Terminal region: no gate onward.
# Same proven lighting. Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[GARDEN] " + m)
def warn(m): unreal.log_warning("[GARDEN] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_FirstGarden_Greybox"
if unreal.EditorAssetLibrary.does_asset_exist(map_pkg):
    unreal.EditorAssetLibrary.delete_asset(map_pkg)
    log("deleted stale level asset")
les.new_level(map_pkg)
log("new level")

CUBE = unreal.load_asset("/Engine/BasicShapes/Cube")
CYL = unreal.load_asset("/Engine/BasicShapes/Cylinder")

def set_mesh(actor, mesh):
    smc = actor.get_component_by_class(unreal.StaticMeshComponent)
    if smc:
        smc.set_static_mesh(mesh)
        try:
            smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
            smc.set_collision_profile_name("BlockAll")
        except Exception as e:
            warn("collision set failed: %s" % e)
    return smc

floor = eas.spawn_actor_from_object(CUBE, unreal.Vector(0.0, 0.0, -50.0))
floor.set_actor_scale3d(unreal.Vector(60.0, 60.0, 1.0))
floor.set_actor_label("Ground")

sun = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 1000.0), unreal.Rotator(-45.0, 0.0, 0.0))
sun.set_actor_label("Sun")
eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0.0, 0.0, 600.0)).set_actor_label("Sky")
eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0)).set_actor_label("SkyAtmosphere")

# The Gardener (data-driven dialogue loads on BeginPlay).
gardener = eas.spawn_actor_from_class(unreal.LongNoonNpc, unreal.Vector(150.0, 0.0, 90.0))
set_mesh(gardener, CYL)
gardener.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.8))
try: gardener.set_editor_property("npc_id", "npc_gardener")
except Exception as e: warn("gardener npc_id failed: %s" % e)
gardener.set_actor_label("Gardener")
log("placed Gardener NPC")

# The three ending choice markers. The Third Way resolves only if the Rememberer
# was found (handled in ALongNoonChoiceMarker via the GameInstance flag).
choices = [
    ("Choice_Give",  unreal.LongNoonChoice.GIVE_IT_BACK,  (450.0, -260.0)),
    ("Choice_Keep",  unreal.LongNoonChoice.KEEP_THE_NOON, (450.0,    0.0)),
    ("Choice_Third", unreal.LongNoonChoice.THE_THIRD_WAY, (450.0,  260.0)),
]
cn = 0
for (label, choice, (x, y)) in choices:
    a = eas.spawn_actor_from_class(unreal.LongNoonChoiceMarker, unreal.Vector(x, y, 90.0))
    set_mesh(a, CYL)
    a.set_actor_scale3d(unreal.Vector(0.8, 0.8, 2.2))
    try: a.set_editor_property("choice", choice)
    except Exception as e: warn("choice failed for %s: %s" % (label, e))
    a.set_actor_label(label); cn += 1
log("placed %d choice markers" % cn)

eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, 0.0, 150.0)).set_actor_label("PlayerStart")

try:
    gm = unreal.load_class(None, "/Game/GameModes/BP_LongNoonGameMode.BP_LongNoonGameMode_C")
    ues.get_editor_world().get_world_settings().set_editor_property("default_game_mode", gm)
    log("gamemode override set")
except Exception as e:
    warn("gamemode override failed: %s" % e)

les.save_current_level()
log("DONE saved " + map_pkg)
unreal.SystemLibrary.quit_editor()
