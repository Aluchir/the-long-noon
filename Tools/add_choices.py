# Place the three ending choice markers in L_FirstGarden_Greybox.
# Run via the FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[CHOICE] " + m)
def warn(m): unreal.log_warning("[CHOICE] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
CYL = unreal.load_asset("/Engine/BasicShapes/Cylinder")

les.load_level("/Game/Maps/L_FirstGarden_Greybox")
log("loaded First Garden")

choices = [
    ("Choice_Give",  unreal.LongNoonChoice.GIVE_IT_BACK,  (300.0, -260.0)),
    ("Choice_Keep",  unreal.LongNoonChoice.KEEP_THE_NOON, (300.0,    0.0)),
    ("Choice_Third", unreal.LongNoonChoice.THE_THIRD_WAY, (300.0,  260.0)),
]
existing = {a.get_actor_label() for a in eas.get_all_level_actors()}
n = 0
for (label, choice, (x, y)) in choices:
    if label in existing:
        log("exists: " + label); continue
    a = eas.spawn_actor_from_class(unreal.LongNoonChoiceMarker, unreal.Vector(x, y, 90.0))
    smc = a.get_component_by_class(unreal.StaticMeshComponent)
    if smc:
        smc.set_static_mesh(CYL)
        try:
            smc.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
            smc.set_collision_profile_name("BlockAll")
        except Exception as e:
            warn("collision: %s" % e)
    a.set_actor_scale3d(unreal.Vector(0.8, 0.8, 2.2))
    try:
        a.set_editor_property("choice", choice)
    except Exception as e:
        warn("set choice failed for %s: %s" % (label, e))
    a.set_actor_label(label)
    n += 1
log("placed %d choice markers" % n)

les.save_current_level()
log("DONE saved First Garden")
unreal.SystemLibrary.quit_editor()
