# Author the 3 missing Enhanced Input actions (Interact/Prune/Sprint), add key
# mappings to IMC_Default (E/F/LeftShift), and assign them on BP_LongNoonCharacter.
# v2: explicitly save duplicated assets to disk, and use UInputMappingContext.map_key
# (the property array reads empty via Python). Run via FULL editor, editor closed first.
import unreal

def log(m): unreal.log("[INPUT] " + m)
def warn(m): unreal.log_warning("[INPUT] " + m)

eal = unreal.EditorAssetLibrary
SRC = "/Game/Input/Actions/IA_Jump"  # Digital(bool) action to clone

new_actions = [
    ("IA_Sprint",   "LeftShift", "sprint_action"),
    ("IA_Interact", "E",         "interact_action"),
    ("IA_Prune",    "F",         "prune_action"),
    ("IA_Pause",    "Escape",    "pause_action"),
    ("IA_Build",    "B",         "build_action"),
    ("IA_BuildCycle", "T",       "build_cycle_action"),
]

# 1. Create + SAVE the action assets.
for (name, _key, _prop) in new_actions:
    dst = "/Game/Input/Actions/" + name
    if not eal.does_asset_exist(dst):
        eal.duplicate_asset(SRC, dst)
    saved = eal.save_asset(dst, only_if_is_dirty=False)
    log("%s exists=%s saved=%s" % (name, eal.does_asset_exist(dst), saved))

# 2. Map keys on IMC_Default via map_key() (fallback to property array).
imc = unreal.load_asset("/Game/Input/IMC_Default")
def make_key(n):
    try: return unreal.Key(n)
    except Exception:
        k = unreal.Key(); k.set_editor_property("key_name", n); return k
for (name, key_name, _prop) in new_actions:
    action = unreal.load_asset("/Game/Input/Actions/" + name)
    try:
        imc.map_key(action, make_key(key_name))
        log("map_key %s -> %s" % (name, key_name))
    except Exception as e:
        warn("map_key failed for %s: %s" % (name, e))
eal.save_asset("/Game/Input/IMC_Default", only_if_is_dirty=False)
try:
    log("IMC mapping count now = %d" % len(imc.get_editor_property("mappings")))
except Exception as e:
    warn("count read failed: %s" % e)

# 3. Assign on the character CDO + save.
char_pkg = "/Game/Characters/BP_LongNoonCharacter"
cdo = unreal.get_default_object(unreal.load_class(None, char_pkg + ".BP_LongNoonCharacter_C"))
for (name, _key, prop) in new_actions:
    a = unreal.load_asset("/Game/Input/Actions/" + name)
    cdo.set_editor_property(prop, a)
    log("char.%s set (asset=%s)" % (prop, a is not None))
eal.save_asset(char_pkg, only_if_is_dirty=False)
log("DONE")
unreal.SystemLibrary.quit_editor()
