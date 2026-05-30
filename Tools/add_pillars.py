# Add reference pillars to L_TestFlat so character translation is visible.
# Run via the FULL editor (-ExecutePythonScript).
import unreal

def log(m): unreal.log("[PILLARS] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

les.load_level("/Game/Maps/L_TestFlat")
log("loaded L_TestFlat")

cube = unreal.load_asset("/Engine/BasicShapes/Cube")
# A row of tall pillars ahead (+X) and to the sides, spaced so walking is obvious.
positions = [
    (500, 0), (1000, 0), (1500, 0), (2000, 0),
    (500, 600), (1000, 600), (500, -600), (1000, -600),
]
existing = {a.get_actor_label() for a in eas.get_all_level_actors()}
i = 0
for (x, y) in positions:
    label = "Pillar_%d" % i
    i += 1
    if label in existing:
        continue
    a = eas.spawn_actor_from_object(cube, unreal.Vector(float(x), float(y), 150.0))
    a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 3.0))  # 100 x 100 x 300 units tall
    a.set_actor_label(label)
log("spawned %d pillars" % i)

les.save_current_level()
log("DONE saved L_TestFlat with pillars")
unreal.SystemLibrary.quit_editor()
