# Swap the remaining primitive-mesh gameplay props across all region maps to thematic
# CC0 meshes (Nature Kit props + Mini Characters for NPCs). Interactables keep BlockAll
# collision (so traces still hit) and auto-scale by bounds. Loads each existing map
# (preserves decor/lights/BGM) and re-saves. Idempotent. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[SWAP] " + m)
def warn(m): unreal.log_warning("[SWAP] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
NK = "/Game/ThirdParty/NatureKit/"
CH = "/Game/ThirdParty/Characters/"

MAPS = ["L_Sunhollow_Greybox", "L_Overgrowth_Greybox", "L_Stillworks_Greybox",
        "L_Hush_Greybox", "L_FirstGarden_Greybox"]

# Per-label mesh + target height. Exact labels first; prefix rules handled below.
EXACT = {
    "GateBuild_0":   (NK + "bridge_wood", 200.0),
    "QuestBoard":    (NK + "sign", 150.0),
    "Rememberer":    (NK + "statue_obelisk", 170.0),
    "Choice_Give":   (NK + "statue_ring", 220.0),
    "Choice_Keep":   (NK + "statue_obelisk", 220.0),
    "Choice_Third":  (NK + "statue_head", 220.0),
    "Tinker":        (CH + "character-male-a", 180.0),
    "Keeper":        (CH + "character-female-b", 180.0),
    "Gardener":      (CH + "character-female-d", 190.0),
}
# Prefix rules (label.startswith -> mesh, height).
PREFIX = [
    ("Lore_", NK + "stone_smallFlatA", 60.0),
]

def mesh_height(mesh):
    try:
        b = mesh.get_bounds()
        return max(1.0, b.box_extent.z * 2.0)
    except Exception:
        return 100.0

def resolve(label):
    if label in EXACT:
        return EXACT[label]
    for (pre, path, h) in PREFIX:
        if label.startswith(pre):
            return (path, h)
    return None

def swap_map(map_pkg):
    les.load_level("/Game/Maps/" + map_pkg)
    n = 0
    for a in eas.get_all_level_actors():
        spec = resolve(a.get_actor_label())
        if not spec:
            continue
        mesh = unreal.load_asset(spec[0])
        if not isinstance(mesh, unreal.StaticMesh):
            warn("missing mesh %s for %s" % (spec[0], a.get_actor_label())); continue
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if not smc:
            continue
        smc.set_static_mesh(mesh)
        s = spec[1] / mesh_height(mesh)
        a.set_actor_scale3d(unreal.Vector(s, s, s))
        # All these are interactables -> keep query collision so traces hit.
        smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
        smc.set_collision_profile_name("BlockAll")
        n += 1
    les.save_current_level()
    log("%s: swapped %d props, saved" % (map_pkg, n))

for m in MAPS:
    swap_map(m)

log("ALL PROPS SWAPPED")
unreal.SystemLibrary.quit_editor()
