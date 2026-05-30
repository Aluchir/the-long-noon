# Add per-region atmosphere polish: an unbound PostProcessVolume (color grading +
# vignette/bloom/exposure) and an ExponentialHeightFog, tuned to each biome's mood:
# Sunhollow warm/bright -> Overgrowth lush/green -> Stillworks desaturated/cool ->
# Hush dark/low-key -> First Garden soft/luminous. Additive: preserves existing
# decor/lights/BGM, only touches actors it owns (label prefix "Atmo_"), so re-runnable.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[ATMO] " + m)
def warn(m): unreal.log_warning("[ATMO] " + m)

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# Per-region mood. color_saturation / color_contrast are unreal.Vector4 (RGB + luminance,
# 1.0 = neutral). vignette/bloom are scalar floats; auto_exposure_bias is EV stops.
# fog_density is small (subtle haze); fog_inscattering is LinearColor (0-1).
REGIONS = {
    "L_Sunhollow_Greybox": {  # warm, bright, gently lifted
        "saturation": (1.05, 1.02, 0.95, 1.0), "contrast": (1.02, 1.0, 0.98, 1.0),
        "vignette": 0.30, "bloom": 0.70, "exposure_bias": 0.20,
        "fog_density": 0.012, "fog_color": (1.0, 0.90, 0.72),
    },
    "L_Overgrowth_Greybox": {  # lush, green-saturated
        "saturation": (0.98, 1.18, 1.0, 1.05), "contrast": (1.0, 1.04, 1.0, 1.0),
        "vignette": 0.35, "bloom": 0.65, "exposure_bias": 0.0,
        "fog_density": 0.020, "fog_color": (0.55, 0.75, 0.50),
    },
    "L_Stillworks_Greybox": {  # desaturated, cool
        "saturation": (0.82, 0.84, 0.90, 0.85), "contrast": (1.05, 1.05, 1.08, 1.05),
        "vignette": 0.40, "bloom": 0.50, "exposure_bias": -0.10,
        "fog_density": 0.018, "fog_color": (0.62, 0.68, 0.78),
    },
    "L_Hush_Greybox": {  # dark, low-key, strong vignette
        "saturation": (0.70, 0.72, 0.82, 0.72), "contrast": (1.12, 1.12, 1.14, 1.12),
        "vignette": 0.70, "bloom": 0.45, "exposure_bias": -0.60,
        "fog_density": 0.045, "fog_color": (0.30, 0.36, 0.50),
    },
    "L_FirstGarden_Greybox": {  # soft, luminous
        "saturation": (1.04, 1.06, 1.02, 1.04), "contrast": (0.97, 0.97, 0.98, 0.97),
        "vignette": 0.20, "bloom": 0.85, "exposure_bias": 0.30,
        "fog_density": 0.010, "fog_color": (0.95, 0.94, 0.85),
    },
}

def trySet(obj, prop, value):
    # PostProcessSettings field names vary across UE 5.x; guard each.
    try:
        obj.set_editor_property(prop, value)
        return True
    except Exception as e:
        warn("set '%s' failed: %s" % (prop, e))
        return False

def grade(settings, cfg):
    # Each graded field needs its override_* bool enabled before the value takes effect.
    sat = cfg["saturation"]; con = cfg["contrast"]
    if trySet(settings, "override_color_saturation", True):
        trySet(settings, "color_saturation", unreal.Vector4(sat[0], sat[1], sat[2], sat[3]))
    if trySet(settings, "override_color_contrast", True):
        trySet(settings, "color_contrast", unreal.Vector4(con[0], con[1], con[2], con[3]))
    if trySet(settings, "override_vignette_intensity", True):
        trySet(settings, "vignette_intensity", float(cfg["vignette"]))
    if trySet(settings, "override_bloom_intensity", True):
        trySet(settings, "bloom_intensity", float(cfg["bloom"]))
    if trySet(settings, "override_auto_exposure_bias", True):
        trySet(settings, "auto_exposure_bias", float(cfg["exposure_bias"]))

def atmosphere(map_pkg, cfg):
    les.load_level("/Game/Maps/" + map_pkg)
    log("polishing " + map_pkg)

    # Idempotent: drop only the actors this script owns.
    removed = 0
    for a in eas.get_all_level_actors():
        if a.get_actor_label().startswith("Atmo_"):
            eas.destroy_actor(a); removed += 1

    # Unbound PostProcessVolume -> applies everywhere in the level.
    ppv = eas.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0.0, 0.0, 0.0),
                                     unreal.Rotator(0.0, 0.0, 0.0))
    trySet(ppv, "unbound", True)
    settings = ppv.get_editor_property("settings")
    grade(settings, cfg)
    # Write the modified struct back in case the getter returned a copy.
    trySet(ppv, "settings", settings)
    ppv.set_actor_label("Atmo_PP")

    # Subtle ExponentialHeightFog for biome haze/color.
    fog = eas.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0.0, 0.0, 0.0),
                                     unreal.Rotator(0.0, 0.0, 0.0))
    fc = cfg["fog_color"]
    comp = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    if comp:
        trySet(comp, "fog_density", float(cfg["fog_density"]))
        trySet(comp, "fog_inscattering_color", unreal.LinearColor(fc[0], fc[1], fc[2], 1.0))
    else:
        warn("no ExponentialHeightFogComponent on spawned fog actor")
    fog.set_actor_label("Atmo_Fog")

    les.save_current_level()
    log("%s: removed %d prior, spawned PP+Fog, saved" % (map_pkg, removed))

for map_pkg, cfg in REGIONS.items():
    atmosphere(map_pkg, cfg)

log("ALL REGIONS POLISHED")
unreal.SystemLibrary.quit_editor()
