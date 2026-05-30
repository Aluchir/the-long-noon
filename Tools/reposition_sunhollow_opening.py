# Tighten the Sunhollow opening so the first prune lands immediately and the gather
# nodes are a step away (no hunting). Repositions EXISTING actors in the live level
# (preserves all decor/lighting/postprocess/audio/mesh-swap layers). The player spawns
# at (0,0) facing +X; the pruning blade reach is 2 m (200 cm) traced straight ahead.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[OPEN] " + m)
def warn(m): unreal.log_warning("[OPEN] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

les.load_level("/Game/Maps/L_Sunhollow_Greybox")
log("loaded Sunhollow")

# Label -> new (x, y). Z is preserved from the actor's current location.
MOVES = {
    "Bloom_0":      (150.0, 0.0),     # dead ahead, inside the 200cm prune reach
    "GatherNode_0": (250.0, 200.0),   # near arc, a small turn + step away
    "GatherNode_1": (250.0, -200.0),
    "GatherNode_2": (440.0, 0.0),     # a few steps ahead
}

moved = 0
for a in eas.get_all_level_actors():
    label = a.get_actor_label()
    if label in MOVES:
        loc = a.get_actor_location()
        x, y = MOVES[label]
        a.set_actor_location(unreal.Vector(x, y, loc.z), False, False)
        log("moved %s -> (%.0f, %.0f, %.0f)" % (label, x, y, loc.z))
        moved += 1

les.save_current_level()
log("DONE moved %d actors, saved" % moved)
unreal.SystemLibrary.quit_editor()
