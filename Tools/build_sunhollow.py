# Build L_Sunhollow_Greybox: a plain (non-WP) greybox of Region 1 to test the
# gather -> prune loop. Floor, golden-hour light, gather nodes (mat_sunmoss),
# a Bloom (Prune), player start, GameMode override. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[SUN] " + m)
def warn(m): unreal.log_warning("[SUN] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_Sunhollow_Greybox"
# Delete any stale/partial asset at this path so new_level saves cleanly to it.
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
        # Block the Visibility channel so the interact/reclaim line traces hit it.
        try:
            smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
            smc.set_collision_profile_name("BlockAll")
        except Exception as e:
            warn("collision set failed: %s" % e)
    return smc

# Ground slab (top at Z=0).
floor = eas.spawn_actor_from_object(CUBE, unreal.Vector(0.0, 0.0, -50.0))
floor.set_actor_scale3d(unreal.Vector(60.0, 60.0, 1.0))
floor.set_actor_label("Ground")

# Golden-hour lighting.
# Match L_TestFlat's proven lighting exactly (no component tweaks).
sun = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 1000.0), unreal.Rotator(-45.0, 0.0, 0.0))
sun.set_actor_label("Sun")
eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0.0, 0.0, 600.0)).set_actor_label("Sky")
eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0)).set_actor_label("SkyAtmosphere")

# Gather nodes (visible spheres) carrying mat_sunmoss.
gather_spots = [(300, 450), (300, -450), (650, 320)]  # off to the sides; the Bloom sits directly ahead
gi = 0
for (x, y) in gather_spots:
    n = eas.spawn_actor_from_class(unreal.GatherNode, unreal.Vector(float(x), float(y), 90.0))
    set_mesh(n, SPHERE)
    n.set_actor_scale3d(unreal.Vector(2.2, 2.2, 2.2))  # tall enough to intersect the eye-line trace
    n.set_actor_label("GatherNode_%d" % gi); gi += 1
    for prop, val in [("item_id", "mat_sunmoss"), ("quantity", 2), ("regrow_seconds", 30.0)]:
        try: n.set_editor_property(prop, val)
        except Exception as e: warn("gather %s failed: %s" % (prop, e))
log("placed %d gather nodes" % gi)

# A Bloom patch to prune (visible cylinder).
bloom = eas.spawn_actor_from_class(unreal.BloomActor, unreal.Vector(320.0, 0.0, 80.0))
set_mesh(bloom, CYL)
bloom.set_actor_label("Bloom_0")
try: bloom.set_editor_property("required_verb", unreal.ReclamationVerb.PRUNE)
except Exception as e: warn("bloom verb failed: %s" % e)
try: bloom.set_editor_property("bloom_tier", 1)
except Exception as e: warn("bloom tier failed: %s" % e)
try: bloom.set_editor_property("drop_table", {"mat_sunmoss": 2})
except Exception as e: warn("bloom drop_table failed: %s" % e)
log("placed bloom")

eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, 0.0, 150.0)).set_actor_label("PlayerStart")

# GameMode override.
try:
    gm = unreal.load_class(None, "/Game/GameModes/BP_LongNoonGameMode.BP_LongNoonGameMode_C")
    ues.get_editor_world().get_world_settings().set_editor_property("default_game_mode", gm)
    log("gamemode override set")
except Exception as e:
    warn("gamemode override failed: %s" % e)

les.save_current_level()
log("DONE saved " + map_pkg)
unreal.SystemLibrary.quit_editor()
