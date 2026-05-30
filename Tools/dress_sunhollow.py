# Dress L_Sunhollow_Greybox with Kenney Nature Kit (CC0) meshes: scatter trees,
# rocks, grass, flowers, and a campfire for a cozy look. Purely additive (decor has
# no collision, so it never breaks the gather/prune/interact traces). Auto-scales each
# mesh from its bounds so the FBX import scale does not matter. Also tints the ground.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal
import math

def log(m): unreal.log("[DRESS] " + m)
def warn(m): unreal.log_warning("[DRESS] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

les.load_level("/Game/Maps/L_Sunhollow_Greybox")
log("loaded Sunhollow")

NK = "/Game/ThirdParty/NatureKit/"

# Clean previous decor (so re-runs do not stack) by label prefix.
removed = 0
for a in eas.get_all_level_actors():
    if a.get_actor_label().startswith("Decor_"):
        eas.destroy_actor(a); removed += 1
if removed:
    log("removed %d old decor actors" % removed)

def mesh_height(mesh):
    try:
        b = mesh.get_bounds()
        return max(1.0, b.box_extent.z * 2.0)
    except Exception:
        return 100.0

decor_n = 0
def place(mesh_name, x, y, target_h, yaw=0.0):
    global decor_n
    mesh = unreal.load_asset(NK + mesh_name)
    if not isinstance(mesh, unreal.StaticMesh):
        warn("not a static mesh (skipped): " + mesh_name); return
    a = eas.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(x, y, 0.0), unreal.Rotator(0.0, yaw, 0.0))
    smc = a.static_mesh_component
    smc.set_static_mesh(mesh)
    s = target_h / mesh_height(mesh)
    a.set_actor_scale3d(unreal.Vector(s, s, s))
    smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)  # decor only
    a.set_actor_label("Decor_%d" % decor_n); decor_n += 1

# A ring of trees around the play space (alternating species), well outside the
# gather/prune area so they frame the scene without blocking interaction.
trees = ["tree_default", "tree_pineDefaultA", "tree_cone", "tree_detailed",
         "tree_default_dark", "tree_pineRoundA", "tree_oak", "tree_plateau"]
ring_r = 900.0
count = 16
for i in range(count):
    ang = (2.0 * math.pi) * (i / float(count))
    x = math.cos(ang) * ring_r
    y = math.sin(ang) * ring_r
    place(trees[i % len(trees)], x, y, 320.0, yaw=(i * 37) % 360)

# Rocks and grass tufts scattered nearer in (still off the central interaction lane).
import_scatter = [
    ("rock_largeA", 600.0, 700.0, 160.0), ("rock_smallB", -650.0, 500.0, 90.0),
    ("rock_largeC", -700.0, -600.0, 170.0), ("rock_smallD", 700.0, -650.0, 90.0),
    ("plant_flatTall", 200.0, 650.0, 70.0), ("plant_flatShort", -250.0, 600.0, 50.0),
    ("plant_flatTall", 500.0, 250.0, 70.0), ("plant_flatShort", -500.0, 250.0, 50.0),
    ("flower_redA", 150.0, -600.0, 60.0), ("flower_yellowA", -150.0, -640.0, 60.0),
    ("mushroom_red", 350.0, 620.0, 50.0), ("mushroom_tan", -350.0, 660.0, 50.0),
    ("plant_bushDetailed", 620.0, 120.0, 110.0), ("plant_bush", -620.0, -120.0, 110.0),
]
for (name, x, y, h) in import_scatter:
    place(name, x, y, h, yaw=(int(x + y)) % 360)

# A cozy campfire between the two NPCs.
place("campfire_logs", 250.0, 0.0, 80.0)

log("placed %d decor actors" % decor_n)

# Tint the ground: find a green Nature Kit material and assign it to the Ground slab.
green_mat = None
for cand in ["grass", "leafsGreen", "leafs", "grass_large"]:
    asset = unreal.load_asset(NK + cand)
    if asset and isinstance(asset, unreal.MaterialInterface):
        green_mat = asset; log("ground material = " + cand); break
if green_mat:
    for a in eas.get_all_level_actors():
        if a.get_actor_label() == "Ground":
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                smc.set_material(0, green_mat)
                log("tinted Ground")
else:
    warn("no green material found; ground left default")

les.save_current_level()
log("DONE saved Sunhollow")
unreal.SystemLibrary.quit_editor()
