using UnrealBuildTool;

public class TheLongNoon : ModuleRules
{
	public TheLongNoon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Add gameplay-systems and UI modules here as they land
			// (UMG for HUD, GameplayTags, etc.).
		});
	}
}
