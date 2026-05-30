# Run several level-build scripts in ONE editor session (saves the ~12s editor
# startup paid per launch). Each per-level script ends with quit_editor(); we strip
# that and quit once at the end. Single editor instance = no asset-lock contention,
# which is exactly why multiple parallel editors do NOT work here.
#
# Usage: pass the scripts to run via the LN_CHAIN env var (semicolon-separated
# absolute paths). If unset, rebuilds all five region greyboxes in chain order.
import unreal
import os

def log(m): unreal.log("[CHAIN] " + m)

TOOLS = "C:/Users/Alu/the-long-noon/Tools"
DEFAULT = [
    TOOLS + "/build_sunhollow.py",
    TOOLS + "/build_overgrowth.py",
    TOOLS + "/build_stillworks.py",
    TOOLS + "/build_hush.py",
    TOOLS + "/build_firstgarden.py",
]

env = os.environ.get("LN_CHAIN", "").strip()
scripts = [s for s in env.split(";") if s] if env else DEFAULT

for path in scripts:
    log("running " + path)
    with open(path, "r", encoding="utf-8") as f:
        src = f.read()
    # Strip the per-script editor quit so the chain continues.
    src = src.replace("unreal.SystemLibrary.quit_editor()", "")
    exec(compile(src, path, "exec"), {})
    log("done " + path)

log("ALL DONE (%d scripts)" % len(scripts))
unreal.SystemLibrary.quit_editor()
