using UnrealBuildTool;

public class TheLongNoon : ModuleRules
{
	public TheLongNoon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Flat module layout (Core/, Player/, Systems/, ... under the module root) with
		// module-root-relative includes like "Data/ToolDef.h". Add the module root so
		// those resolve without a Public/Private split.
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Add gameplay-systems and UI modules here as they land
			// (UMG for HUD, GameplayTags, etc.).
		});
	}
}
