import unreal
imc = unreal.load_asset("/Game/Input/IMC_Default")
mappings = imc.get_editor_property("mappings")
unreal.log("[IMC] count=%d" % len(mappings))
for m in mappings:
    a = m.get_editor_property("action")
    k = m.get_editor_property("key")
    aname = a.get_name() if a else "None"
    try:
        kname = str(k.get_editor_property("key_name"))
    except Exception:
        kname = str(k)
    unreal.log("[IMC] %s -> %s" % (aname, kname))
unreal.SystemLibrary.quit_editor()
