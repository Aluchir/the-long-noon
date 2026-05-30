# Set every region's DirectionalLight + SkyLight to Movable (fully dynamic -> clears
# the "LIGHTING NEEDS TO BE REBUILT" warning, no bake needed) and tint the sun per
# region for the narrative light-dial: golden (Sunhollow) -> bronze (Overgrowth) ->
# pale (Stillworks) -> cold/dim (Hush) -> restored soft-gold (First Garden).
# Loads each existing map (preserves decor) and re-saves. Run via FULL editor
# (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[LIGHT] " + m)
def warn(m): unreal.log_warning("[LIGHT] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# (sun rgb 0-255, sun intensity lux). Intensities kept in a safe-visible band; the
# dial reads mostly through hue + a gentle dim toward the Hush.
REGIONS = {
    "L_Sunhollow_Greybox":   ((255, 224, 170), 6.0),
    "L_Overgrowth_Greybox":  ((255, 198, 130), 5.5),
    "L_Stillworks_Greybox":  ((212, 216, 228), 5.0),
    "L_Hush_Greybox":        ((158, 172, 210), 4.0),
    "L_FirstGarden_Greybox": ((255, 240, 216), 6.0),
}

def relight(map_pkg, rgb, intensity):
    les.load_level("/Game/Maps/" + map_pkg)
    dl = sl = 0
    for a in eas.get_all_level_actors():
        if isinstance(a, unreal.DirectionalLight):
            comp = a.get_component_by_class(unreal.DirectionalLightComponent)
            if comp:
                a.root_component.set_mobility(unreal.ComponentMobility.MOVABLE)
                comp.set_intensity(intensity)
                comp.set_light_color(unreal.LinearColor(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0, 1.0))
                dl += 1
        elif isinstance(a, unreal.SkyLight):
            comp = a.get_component_by_class(unreal.SkyLightComponent)
            if comp:
                a.root_component.set_mobility(unreal.ComponentMobility.MOVABLE)
                try:
                    comp.set_editor_property("real_time_capture", True)
                except Exception as e:
                    warn("skylight rtc: %s" % e)
                comp.recapture_sky()
                sl += 1
    les.save_current_level()
    log("%s: sun=%d sky=%d -> rgb%s i=%.1f, saved" % (map_pkg, dl, sl, rgb, intensity))

for map_pkg, (rgb, i) in REGIONS.items():
    relight(map_pkg, rgb, i)

log("ALL REGIONS RELIT")
unreal.SystemLibrary.quit_editor()
