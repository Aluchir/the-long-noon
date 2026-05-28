using Godot;

namespace TheLongNoon.Core;

/// <summary>
/// Top-level game state singleton (autoload). Owns the high-level game phase,
/// the active save slot, and global services that systems query. Kept thin:
/// gameplay logic lives in the per-system scripts under src/systems/.
/// See docs/tech/coding-standards-and-architecture.md.
/// </summary>
public partial class GameManager : Node
{
    public static GameManager Instance { get; private set; } = null!;

    /// <summary>Coarse game phase. Region/state progression lives in the region system.</summary>
    public enum GamePhase { Boot, MainMenu, Playing, Paused }

    public GamePhase Phase { get; private set; } = GamePhase.Boot;

    public override void _Ready()
    {
        Instance = this;
        Phase = GamePhase.MainMenu;
        GD.Print("[GameManager] ready.");
    }

    public void SetPhase(GamePhase phase)
    {
        Phase = phase;
        EventBus.Instance?.EmitGamePhaseChanged((int)phase);
    }
}
