using UnrealBuildTool;

public class LyokoEditor : ModuleRules
{
    public LyokoEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UnrealEd",         // Required for most editor functionalities
                "Slate",
                "SlateCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Lyoko",
                "EditorFramework",
                "LevelEditor",      // For menu extensions or editor hooks
                "AssetTools",       // For custom asset types
                "PropertyEditor",   // For custom details panels
                "Projects"
            }
        );
    }
}
