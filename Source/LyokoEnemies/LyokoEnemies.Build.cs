using UnrealBuildTool;

public class LyokoEnemies : ModuleRules
{
    public LyokoEnemies(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine",
            "InputCore",
            "EnhancedInput",
            "LyokoBase",
            "LyokoAI"
        });
    }
}
