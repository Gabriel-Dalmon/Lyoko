using UnrealBuildTool;

public class LyokoBase : ModuleRules
{
    public LyokoBase(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine",
            "InputCore",
            "EnhancedInput"
        });
    }
}
