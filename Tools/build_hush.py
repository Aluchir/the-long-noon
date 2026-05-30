# Build L_Hush_Greybox: a plain (non-WP) stub of Region 4 (the Hush), reached by
# building the Stillworks gate. Holds the optional Rememberer fragment that unlocks
# the Third Way ending. Same proven lighting as the other greyboxes. Run via FULL
# editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[HUSH] " + m)
def warn(m): unreal.log_warning("[HUSH] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_Hush_Greybox"
if unreal.EditorAssetLibrary.does_asset_exist(map_pkg):
    unreal.EditorAssetLibrary.delete_asset(map_pkg)
    log("deleted stale level asset")
les.new_level(map_pkg)
log("new level")

CUBE = unreal.load_asset("/Engine/BasicShapes/Cube")
SPHERE = unreal.load_asset("/Engine/BasicShapes/Sphere")
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

gi = 0
for (x, y) in [(300, 400), (300, -400)]:
    n = eas.spawn_actor_from_class(unreal.GatherNode, unreal.Vector(float(x), float(y), 90.0))
    set_mesh(n, SPHERE)
    n.set_actor_scale3d(unreal.Vector(2.2, 2.2, 2.2))
    n.set_actor_label("GatherNode_%d" % gi)
    props = [("item_id", "mat_hushglass"), ("quantity", 2), ("regrow_seconds", 30.0)]
    if gi == 0:
        props.append(("completes_objective", "gather_hushglass"))
    for prop, val in props:
        try: n.set_editor_property(prop, val)
        except Exception as e: warn("gather %s failed: %s" % (prop, e))
    gi += 1
log("placed %d gather nodes" % gi)

bloom = eas.spawn_actor_from_class(unreal.BloomActor, unreal.Vector(320.0, 0.0, 80.0))
set_mesh(bloom, CYL)
bloom.set_actor_label("Bloom_0")
try: bloom.set_editor_property("required_verb", unreal.ReclamationVerb.STILL)
except Exception as e: warn("bloom verb failed: %s" % e)
try: bloom.set_editor_property("bloom_tier", 4)
except Exception as e: warn("bloom tier failed: %s" % e)
try: bloom.set_editor_property("drop_table", {"mat_hushglass": 2})
except Exception as e: warn("bloom drop_table failed: %s" % e)
try: bloom.set_editor_property("completes_objective", "still_hush")
except Exception as e: warn("bloom completes_objective failed: %s" % e)
log("placed bloom")

# A gate onward (toward the First Garden), opening the final region.
gate = eas.spawn_actor_from_class(unreal.LongNoonGateBuild, unreal.Vector(700.0, 0.0, 90.0))
set_mesh(gate, CUBE)
gate.set_actor_scale3d(unreal.Vector(0.6, 3.0, 1.4))
gate.set_actor_label("GateBuild_0")
for prop, val in [("gate_build_id", "gate_firstgarden"), ("completes_objective", "build_gate_4"),
                  ("target_level", "L_FirstGarden_Greybox")]:
    try: gate.set_editor_property(prop, val)
    except Exception as e: warn("gate %s failed: %s" % (prop, e))
log("placed gate build")

board = eas.spawn_actor_from_class(unreal.LongNoonQuestBoard, unreal.Vector(120.0, 0.0, 90.0))
set_mesh(board, CUBE)
board.set_actor_scale3d(unreal.Vector(0.3, 1.2, 1.6))
board.set_actor_label("QuestBoard")
try:
    board.set_editor_property("objectives", ["gather_hushglass", "still_hush", "build_gate_4"])
except Exception as e:
    warn("quest board objectives failed: %s" % e)
log("placed quest board")

# The Rememberer: an OPTIONAL deep-lore find. Reading their fragment marks the
# Rememberer found, unlocking the Third Way ending. Tucked off to the side.
rem = eas.spawn_actor_from_class(unreal.LongNoonLoreFragment, unreal.Vector(150.0, 500.0, 95.0))
set_mesh(rem, CUBE)
rem.set_actor_scale3d(unreal.Vector(0.6, 0.6, 1.0))
rem.set_actor_label("Rememberer")
for prop, val in [("fragment_id", "frag_remembrancer_archive"), ("grants_rememberer", True)]:
    try: rem.set_editor_property(prop, val)
    except Exception as e: warn("rememberer %s failed: %s" % (prop, e))
log("placed Rememberer fragment")

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
