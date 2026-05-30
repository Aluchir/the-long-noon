# Asset Drop

Drop free/CC0 pack files here (FBX/glTF/OBJ, PNG/JPG, WAV/OGG), then run:

    UnrealEditor.exe TheLongNoon.uproject -ExecutePythonScript=Tools/import_assets.py -unattended -nosplash

They import into /Game/ThirdParty mirroring the folder structure. Fab/Marketplace
packs (already .uasset) go straight into Content/ via "Add to Project" - no import needed.
