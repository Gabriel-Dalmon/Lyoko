using UnrealBuildTool;

public class LyokoDungeon : ModuleRules
{
    public LyokoDungeon(ReadOnlyTargetRules Target) : base(Target)
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
