using UnrealBuildTool;

public class LyokoPlayer : ModuleRules
{
    public LyokoPlayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine",
            "InputCore",
            "EnhancedInput",
            "LyokoBase"
        });
    }
}
