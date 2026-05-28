using Godot;

namespace TheLongNoon.Core;

/// <summary>
/// Global signal hub (autoload). Systems communicate through decoupled signals
/// rather than direct references, so e.g. the codex can react to "fragment found"
/// without the world scene knowing the codex exists.
/// See docs/tech/coding-standards-and-architecture.md.
/// </summary>
public partial class EventBus : Node
{
    public static EventBus Instance { get; private set; } = null!;

    [Signal] public delegate void GamePhaseChangedEventHandler(int phase);
    [Signal] public delegate void LoreFragmentFoundEventHandler(string fragmentId);
    [Signal] public delegate void ScriptLiteracyChangedEventHandler(int tier);
    [Signal] public delegate void RegionUnlockedEventHandler(string regionId);
    [Signal] public delegate void ItemCraftedEventHandler(string itemId);

    public override void _Ready()
    {
        Instance = this;
        GD.Print("[EventBus] ready.");
    }

    public void EmitGamePhaseChanged(int phase) => EmitSignal(SignalName.GamePhaseChanged, phase);
    public void EmitLoreFragmentFound(string id) => EmitSignal(SignalName.LoreFragmentFound, id);
    public void EmitScriptLiteracyChanged(int tier) => EmitSignal(SignalName.ScriptLiteracyChanged, tier);
    public void EmitRegionUnlocked(string id) => EmitSignal(SignalName.RegionUnlocked, id);
    public void EmitItemCrafted(string id) => EmitSignal(SignalName.ItemCrafted, id);
}
