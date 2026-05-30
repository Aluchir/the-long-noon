# Build L_Overgrowth_Greybox: a plain (non-WP) stub of Region 2 (the Overgrowth),
# reached by building the Sunhollow gate. Proves region transition. Uses the SAME
# proven lighting as Sunhollow (default lights + Rotator(-45,0,0)); the bronze
# light-dial art pass is a separate art task. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[OVG] " + m)
def warn(m): unreal.log_warning("[OVG] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

map_pkg = "/Game/Maps/L_Overgrowth_Greybox"
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

# Ground slab (top at Z=0).
floor = eas.spawn_actor_from_object(CUBE, unreal.Vector(0.0, 0.0, -50.0))
floor.set_actor_scale3d(unreal.Vector(60.0, 60.0, 1.0))
floor.set_actor_label("Ground")

# Proven lighting (do NOT tweak color/rotation; that caused black scenes before).
sun = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 1000.0), unreal.Rotator(-45.0, 0.0, 0.0))
sun.set_actor_label("Sun")
eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0.0, 0.0, 600.0)).set_actor_label("Sky")
eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0.0, 0.0, 0.0)).set_actor_label("SkyAtmosphere")

# A couple of R2 gather nodes (mat_swallowedwood) and a tougher Bloom to settle.
gi = 0
for (x, y) in [(300, 400), (300, -400)]:
    n = eas.spawn_actor_from_class(unreal.GatherNode, unreal.Vector(float(x), float(y), 90.0))
    set_mesh(n, SPHERE)
    n.set_actor_scale3d(unreal.Vector(2.2, 2.2, 2.2))
    n.set_actor_label("GatherNode_%d" % gi); gi += 1
    for prop, val in [("item_id", "mat_swallowedwood"), ("quantity", 2), ("regrow_seconds", 30.0)]:
        try: n.set_editor_property(prop, val)
        except Exception as e: warn("gather %s failed: %s" % (prop, e))
log("placed %d gather nodes" % gi)

bloom = eas.spawn_actor_from_class(unreal.BloomActor, unreal.Vector(320.0, 0.0, 80.0))
set_mesh(bloom, CYL)
bloom.set_actor_label("Bloom_0")
try: bloom.set_editor_property("required_verb", unreal.ReclamationVerb.SETTLE)
except Exception as e: warn("bloom verb failed: %s" % e)
try: bloom.set_editor_property("bloom_tier", 2)
except Exception as e: warn("bloom tier failed: %s" % e)
try: bloom.set_editor_property("drop_table", {"mat_swallowedwood": 2})
except Exception as e: warn("bloom drop_table failed: %s" % e)
log("placed bloom")

# A lore fragment hinting at the deeper regions.
lf = eas.spawn_actor_from_class(unreal.LongNoonLoreFragment, unreal.Vector(420.0, 160.0, 95.0))
set_mesh(lf, CUBE)
lf.set_actor_scale3d(unreal.Vector(0.5, 0.5, 0.8))
try: lf.set_editor_property("fragment_id", "frag_overgrowth_roots")
except Exception as e: warn("fragment_id failed: %s" % e)
lf.set_actor_label("Lore_0")
log("placed lore fragment")

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
