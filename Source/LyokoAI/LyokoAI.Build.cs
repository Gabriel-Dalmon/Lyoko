using UnrealBuildTool;

public class LyokoAI : ModuleRules
{
    public LyokoAI(ReadOnlyTargetRules Target) : base(Target)
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
