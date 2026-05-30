# Dress the non-Sunhollow region greyboxes with Kenney Nature Kit (CC0) meshes, giving
# each a distinct biome palette. Same approach as dress_sunhollow.py: scatter decor
# (NO_COLLISION), swap GatherNode_*/Bloom_0 to nature meshes (keep BlockAll + auto-scale),
# tint the ground. Idempotent (clears Decor_* first). Runs all four maps in ONE editor
# session. Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal
import math

def log(m): unreal.log("[DRESSR] " + m)
def warn(m): unreal.log_warning("[DRESSR] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
NK = "/Game/ThirdParty/NatureKit/"

# Per-region biome: ring trees, scatter props, gather/bloom swaps, ground material.
REGIONS = {
    "L_Overgrowth_Greybox": {
        "trees": ["tree_oak", "tree_detailed", "tree_fat", "tree_default", "tree_thin"],
        "scatter": [("plant_bushLarge", 600, 700, 140), ("plant_bushDetailed", -650, 500, 130),
                    ("rock_largeB", -700, -600, 170), ("mushroom_redGroup", 700, -650, 90),
                    ("flower_purpleA", 200, 650, 60), ("plant_bushTriangle", -250, 600, 120),
                    ("plant_flatTall", 500, 250, 70), ("flower_redB", -500, 250, 60)],
        "gather": "plant_bushLarge", "bloom": "tree_fat", "ground": "leafsGreen",
    },
    "L_Stillworks_Greybox": {
        "trees": ["tree_thin", "tree_thin_dark", "tree_plateau", "tree_simple_dark", "tree_blocks"],
        "scatter": [("rock_largeA", 600, 700, 200), ("rock_largeC", -650, 500, 200),
                    ("rock_largeD", -700, -600, 190), ("rock_smallB", 700, -650, 90),
                    ("rock_smallC", 200, 650, 80), ("rock_largeE", -250, 600, 200),
                    ("stump_old", 500, 250, 80), ("rock_smallD", -500, 250, 80)],
        "gather": "rock_largeF", "bloom": "rock_largeA", "ground": "stone",
    },
    "L_Hush_Greybox": {
        "trees": ["tree_pineTallA", "tree_pineTallB", "tree_pineRoundA", "tree_pineSmallA", "tree_cone_dark"],
        "scatter": [("mushroom_tanGroup", 600, 700, 90), ("mushroom_redTall", -650, 500, 90),
                    ("plant_bushSmall", -700, -600, 100), ("rock_smallA", 700, -650, 90),
                    ("grass_leafs", 200, 650, 60), ("plant_bush", -250, 600, 110),
                    ("mushroom_tan", 500, 250, 50), ("plant_bushSmall", -500, 250, 100)],
        "gather": "mushroom_tanGroup", "bloom": "plant_bushLargeTriangle", "ground": "leafsDark",
    },
    "L_FirstGarden_Greybox": {
        "trees": ["tree_default", "tree_cone", "tree_oak", "tree_detailed", "tree_small"],
        "scatter": [("flower_redA", 600, 300, 70), ("flower_yellowA", -600, 300, 70),
                    ("flower_purpleA", 600, -300, 70), ("flower_redC", -600, -300, 70),
                    ("flower_yellowB", 250, 600, 70), ("flower_purpleB", -250, 600, 70),
                    ("plant_bushDetailed", 700, 0, 120), ("plant_bushLarge", -700, 0, 130)],
        "gather": None, "bloom": None, "ground": "leafsGreen",
    },
}

def mesh_height(mesh):
    try:
        b = mesh.get_bounds()
        return max(1.0, b.box_extent.z * 2.0)
    except Exception:
        return 100.0

def dress(map_pkg, cfg):
    les.load_level("/Game/Maps/" + map_pkg)
    log("dressing " + map_pkg)

    removed = 0
    for a in eas.get_all_level_actors():
        if a.get_actor_label().startswith("Decor_"):
            eas.destroy_actor(a); removed += 1

    n = [0]
    def place(mesh_name, x, y, target_h, yaw=0.0):
        mesh = unreal.load_asset(NK + mesh_name)
        if not isinstance(mesh, unreal.StaticMesh):
            warn("skip (not a mesh): " + mesh_name); return
        a = eas.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(float(x), float(y), 0.0),
                                       unreal.Rotator(0.0, yaw, 0.0))
        smc = a.static_mesh_component
        smc.set_static_mesh(mesh)
        s = target_h / mesh_height(mesh)
        a.set_actor_scale3d(unreal.Vector(s, s, s))
        smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        a.set_actor_label("Decor_%d" % n[0]); n[0] += 1

    trees = cfg["trees"]
    ring_r, count = 900.0, 16
    for i in range(count):
        ang = (2.0 * math.pi) * (i / float(count))
        place(trees[i % len(trees)], math.cos(ang) * ring_r, math.sin(ang) * ring_r, 320.0, (i * 37) % 360)
    # Forest backdrop ahead (+X).
    for (x, y, h) in [(1050, 250, 360), (1050, -250, 360), (1250, 0, 440), (950, 480, 320), (950, -480, 320)]:
        place(trees[(x + y) % len(trees)], x, y, h, (x + y) % 360)
    for (name, x, y, h) in cfg["scatter"]:
        place(name, x, y, h, (int(x + y)) % 360)
    log("%s: placed %d decor" % (map_pkg, n[0]))

    def swap(label, mesh_name, target_h):
        if not mesh_name:
            return
        mesh = unreal.load_asset(NK + mesh_name)
        if not isinstance(mesh, unreal.StaticMesh):
            warn("swap skip (not a mesh): " + mesh_name); return
        for a in eas.get_all_level_actors():
            if a.get_actor_label() == label:
                smc = a.get_component_by_class(unreal.StaticMeshComponent)
                if smc:
                    smc.set_static_mesh(mesh)
                    s = target_h / mesh_height(mesh)
                    a.set_actor_scale3d(unreal.Vector(s, s, s))
                    smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
                    smc.set_collision_profile_name("BlockAll")
                return
    swap("GatherNode_0", cfg["gather"], 170.0)
    swap("GatherNode_1", cfg["gather"], 170.0)
    swap("Bloom_0", cfg["bloom"], 190.0)

    green = unreal.load_asset(NK + cfg["ground"])
    if isinstance(green, unreal.MaterialInterface):
        for a in eas.get_all_level_actors():
            if a.get_actor_label() == "Ground":
                smc = a.get_component_by_class(unreal.StaticMeshComponent)
                if smc:
                    smc.set_material(0, green)
    else:
        warn("ground material '%s' not found for %s" % (cfg["ground"], map_pkg))

    les.save_current_level()
    log("%s: DONE saved" % map_pkg)

for map_pkg, cfg in REGIONS.items():
    dress(map_pkg, cfg)

log("ALL REGIONS DRESSED")
unreal.SystemLibrary.quit_editor()
