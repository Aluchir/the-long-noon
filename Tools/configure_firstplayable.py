# Headless first-playable configuration for The Long Noon (asset ops only).
# Run via: UnrealEditor-Cmd.exe <uproject> -run=pythonscript -script="<this file>"
# Creates BP_LongNoonGameMode (pawn = BP_LongNoonCharacter) and wires the character's
# Enhanced Input. NO level-editor subsystem calls (those crash in commandlet mode);
# the playable level is the existing template map, with our GameMode forced via the
# launch URL (?game=...). Idempotent.

import unreal

def log(msg):
    unreal.log("[CFG] " + msg)

def warn(msg):
    unreal.log_warning("[CFG] " + msg)

log("script start")

ar = unreal.AssetRegistryHelpers.get_asset_registry()

def find_pkg(short_name):
    """Return the package path of the first /Game asset whose name matches, else None."""
    f = unreal.ARFilter(package_paths=["/Game"], recursive_paths=True)
    for a in ar.get_assets(f):
        if str(a.asset_name) == short_name:
            return str(a.package_name)
    return None

def load_gen_class(bp_pkg):
    name = bp_pkg.rsplit("/", 1)[-1]
    return unreal.load_class(None, bp_pkg + "." + name + "_C")

# --- 1. Locate the user's character Blueprint -------------------------------
char_pkg = find_pkg("BP_LongNoonCharacter")
if not char_pkg:
    warn("BP_LongNoonCharacter not found under /Game. Aborting.")
    raise SystemExit(1)
log("character BP: " + char_pkg)
char_class = load_gen_class(char_pkg)

# --- 2. Wire Enhanced Input on the character CDO (defensive/idempotent) ------
def set_if_found(cdo, prop, asset_name):
    pkg = find_pkg(asset_name)
    if pkg:
        try:
            cdo.set_editor_property(prop, unreal.load_asset(pkg))
            log("set %s = %s" % (prop, asset_name))
        except Exception as e:
            warn("could not set %s: %s" % (prop, e))
    else:
        warn("input asset not found: %s (skipping %s)" % (asset_name, prop))

try:
    char_cdo = unreal.get_default_object(char_class)
    set_if_found(char_cdo, "default_mapping_context", "IMC_Default")
    set_if_found(char_cdo, "move_action", "IA_Move")
    set_if_found(char_cdo, "look_action", "IA_Look")
    set_if_found(char_cdo, "jump_action", "IA_Jump")
    unreal.EditorAssetLibrary.save_asset(char_pkg, only_if_is_dirty=False)
    log("character input wired + saved")
except Exception as e:
    warn("input wiring failed: %s" % e)

# --- 3. Create or update the GameMode Blueprint with our pawn ----------------
gm_pkg = "/Game/GameModes/BP_LongNoonGameMode"
gm_native = unreal.load_class(None, "/Script/TheLongNoon.LongNoonGameMode")
if not unreal.EditorAssetLibrary.does_asset_exist(gm_pkg):
    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", gm_native)
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    tools.create_asset("BP_LongNoonGameMode", "/Game/GameModes", unreal.Blueprint, factory)
    log("created " + gm_pkg)
else:
    log("gamemode already exists: " + gm_pkg)

gm_class = load_gen_class(gm_pkg)
gm_cdo = unreal.get_default_object(gm_class)
gm_cdo.set_editor_property("default_pawn_class", char_class)
unreal.EditorAssetLibrary.save_asset(gm_pkg, only_if_is_dirty=False)
log("gamemode pawn set to BP_LongNoonCharacter + saved")

log("DONE: gamemode + input configured. GameMode class = %s.%s_C" % (gm_pkg, gm_pkg.rsplit('/', 1)[-1]))
