using UnrealBuildTool;

public class LyokoWeapons : ModuleRules
{
    public LyokoWeapons(ReadOnlyTargetRules Target) : base(Target)
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
