using UnrealBuildTool;
using System.Collections.Generic;

public class TheLongNoonTarget : TargetRules
{
	public TheLongNoonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("TheLongNoon");
	}
}
