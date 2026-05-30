# Build greybox levels for regions R2-R5 (plain, non-WP). Same safe lighting as
# Sunhollow (per-region light-dial/palette is a deferred art pass); each gets its
# signature gather material + spawn + GameMode override. Run via FULL editor.
import unreal

def log(m): unreal.log("[REGION] " + m)
def warn(m): unreal.log_warning("[REGION] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
ues = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)

CUBE = unreal.load_asset("/Engine/BasicShapes/Cube")
SPHERE = unreal.load_asset("/Engine/BasicShapes/Sphere")
GM = unreal.load_class(None, "/Game/GameModes/BP_LongNoonGameMode.BP_LongNoonGameMode_C")

def set_mesh(actor, mesh):
    smc = actor.get_component_by_class(unreal.StaticMeshComponent)
    if smc:
        smc.set_static_mesh(mesh)
        try:
            smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
            smc.set_collision_profile_name("BlockAll")
        except Exception as e:
            warn("collision: %s" % e)
    return smc

# (map package, signature material id, npc id or None)
REGIONS = [
    ("/Game/Maps/L_Overgrowth_Greybox",   "mat_swallowedwood", "npc_forager"),
    ("/Game/Maps/L_Stillworks_Greybox",   "mat_stillsteel",    "npc_one_who_counts"),
    ("/Game/Maps/L_Hush_Greybox",         "mat_hushglass",     "npc_rememberer"),
    ("/Game/Maps/L_FirstGarden_Greybox",  "mat_gardenlight",   "npc_gardener"),
]

for (pkg, mat, npc) in REGIONS:
    if unreal.EditorAssetLibrary.does_asset_exist(pkg):
        unreal.EditorAssetLibrary.delete_asset(pkg)
    les.new_level(pkg)

    floor = eas.spawn_actor_from_object(CUBE, unreal.Vector(0.0, 0.0, -50.0))
    floor.set_actor_scale3d(unreal.Vector(60.0, 60.0, 1.0)); floor.set_actor_label("Ground")
    eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 1000.0), unreal.Rotator(-45.0, 0.0, 0.0)).set_actor_label("Sun")
    eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 600.0)).set_actor_label("Sky")
    eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0)).set_actor_label("SkyAtmosphere")
    eas.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 150.0)).set_actor_label("PlayerStart")

    for i, (gx, gy) in enumerate([(350, 250), (350, -250)]):
        n = eas.spawn_actor_from_class(unreal.GatherNode, unreal.Vector(float(gx), float(gy), 90.0))
        set_mesh(n, SPHERE); n.set_actor_scale3d(unreal.Vector(2.2, 2.2, 2.2))
        n.set_actor_label("GatherNode_%d" % i)
        try:
            n.set_editor_property("item_id", mat); n.set_editor_property("quantity", 2)
        except Exception as e:
            warn("node prop: %s" % e)

    if npc:
        a = eas.spawn_actor_from_class(unreal.LongNoonNpc, unreal.Vector(250.0, 0.0, 90.0))
        set_mesh(a, unreal.load_asset("/Engine/BasicShapes/Cylinder"))
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.8))
        try: a.set_editor_property("npc_id", npc)
        except Exception as e: warn("npc: %s" % e)
        a.set_actor_label(npc)

    try:
        ues.get_editor_world().get_world_settings().set_editor_property("default_game_mode", GM)
    except Exception as e:
        warn("gamemode: %s" % e)
    les.save_current_level()
    log("built %s (mat=%s npc=%s)" % (pkg, mat, npc))

log("DONE all regions")
unreal.SystemLibrary.quit_editor()
