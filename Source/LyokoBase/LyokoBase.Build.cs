using UnrealBuildTool;

public class LyokoBase : ModuleRules
{
    public LyokoBase(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] {
            "CommonInput" //for LyokoInputDeviceSubsystem 
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Slate",
            "SlateCore",
            "GameplayTags",
            "Niagara"
        });
    }
}
