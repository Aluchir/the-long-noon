# Author the 3 missing Enhanced Input actions (Interact/Prune/Sprint), add key
# mappings to IMC_Default (E/F/LeftShift), and assign them on BP_LongNoonCharacter.
# Run via FULL editor (-ExecutePythonScript), editor closed first.
import unreal

def log(m): unreal.log("[INPUT] " + m)
def warn(m): unreal.log_warning("[INPUT] " + m)

eal = unreal.EditorAssetLibrary
SRC = "/Game/Input/Actions/IA_Jump"  # a Digital(bool) action to clone

new_actions = [
    ("IA_Sprint",   "LeftShift", "sprint_action"),
    ("IA_Interact", "E",         "interact_action"),
    ("IA_Prune",    "F",         "prune_action"),
]

# 1. Create the action assets by duplicating the bool jump action.
for (name, _key, _prop) in new_actions:
    dst = "/Game/Input/Actions/" + name
    if not eal.does_asset_exist(dst):
        ok = eal.duplicate_asset(SRC, dst)
        log("duplicated -> %s (%s)" % (name, "ok" if ok else "FAILED"))
    else:
        log("exists: " + name)

# 2. Add key mappings to IMC_Default.
imc = unreal.load_asset("/Game/Input/IMC_Default")

def make_key(key_name):
    # Try the common FKey constructions.
    try:
        return unreal.Key(key_name)
    except Exception:
        k = unreal.Key()
        k.set_editor_property("key_name", key_name)
        return k

try:
    mappings = imc.get_editor_property("mappings")
    for (name, key_name, _prop) in new_actions:
        action = unreal.load_asset("/Game/Input/Actions/" + name)
        m = unreal.EnhancedActionKeyMapping()
        m.set_editor_property("action", action)
        m.set_editor_property("key", make_key(key_name))
        mappings.append(m)
        log("mapped %s -> %s" % (name, key_name))
    imc.set_editor_property("mappings", mappings)
    eal.save_loaded_asset(imc)
    log("IMC_Default saved with %d mappings" % len(mappings))
except Exception as e:
    warn("IMC mapping failed: %s" % e)

# 3. Assign the actions on the character CDO.
char_pkg = "/Game/Characters/BP_LongNoonCharacter"
cls = unreal.load_class(None, char_pkg + ".BP_LongNoonCharacter_C")
cdo = unreal.get_default_object(cls)
for (name, _key, prop) in new_actions:
    try:
        cdo.set_editor_property(prop, unreal.load_asset("/Game/Input/Actions/" + name))
        log("char.%s = %s" % (prop, name))
    except Exception as e:
        warn("assign %s failed: %s" % (prop, e))
eal.save_asset(char_pkg, only_if_is_dirty=False)
log("DONE")
unreal.SystemLibrary.quit_editor()
