# Place a crafting bench in Sunhollow next to the Tinker (the mentor crafter). Live-map
# add (preserves decor/lighting/audio layers). Interact (E) opens the crafting menu.
# Uses a Nature Kit stump as the rustic worktable. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[CRAFT] " + m)
def warn(m): unreal.log_warning("[CRAFT] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

les.load_level("/Game/Maps/L_Sunhollow_Greybox")
log("loaded Sunhollow")

# Clean any prior bench (idempotent).
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "CraftBench":
        eas.destroy_actor(a)

station = eas.spawn_actor_from_class(unreal.LongNoonCraftingStation, unreal.Vector(180.0, 300.0, 0.0))
station.set_actor_label("CraftBench")
smc = station.get_component_by_class(unreal.StaticMeshComponent)
mesh = unreal.load_asset("/Game/ThirdParty/NatureKit/stump_old")
if isinstance(mesh, unreal.StaticMesh) and smc:
    smc.set_static_mesh(mesh)
    # Scale to a usable, trace-hittable size and block the interact line trace.
    b = mesh.get_bounds()
    h = max(1.0, b.box_extent.z * 2.0)
    s = 160.0 / h
    station.set_actor_scale3d(unreal.Vector(s * 1.4, s * 1.4, s))
    smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
    smc.set_collision_profile_name("BlockAll")
else:
    warn("stump_old not found; bench has no mesh")

try:
    station.set_editor_property("station_id", "station_basic_bench")
except Exception as e:
    warn("station_id failed: %s" % e)

log("placed CraftBench (station_basic_bench) near the Tinker")
les.save_current_level()
log("DONE saved")
unreal.SystemLibrary.quit_editor()
