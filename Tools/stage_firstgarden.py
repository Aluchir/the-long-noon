# Stage the First Garden climax: the Gardener stands to one side (not blocking the
# choices), and the three ending choice markers form an arc the player faces, with
# "Keep the Long Noon" dead ahead. Live-map edit (preserves art/lighting/audio).
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[GARDEN2] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

les.load_level("/Game/Maps/L_FirstGarden_Greybox")

MOVES = {
    "Gardener":     (260.0, 300.0),   # off to the side, greeting from the edge
    "Choice_Give":  (210.0, -190.0),  # left, within a step of spawn
    "Choice_Keep":  (210.0, 0.0),     # dead ahead, reachable
    "Choice_Third": (210.0, 190.0),   # right
}
moved = 0
for a in eas.get_all_level_actors():
    label = a.get_actor_label()
    if label in MOVES:
        loc = a.get_actor_location()
        x, y = MOVES[label]
        a.set_actor_location(unreal.Vector(x, y, loc.z), False, False)
        log("moved %s -> (%.0f, %.0f)" % (label, x, y))
        moved += 1

les.save_current_level()
log("DONE moved %d, saved" % moved)
unreal.SystemLibrary.quit_editor()
