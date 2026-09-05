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
            "MassEntity",
            "GeometryCollectionEngine",
            "GameplayTags",
            "LyokoBase"
        });

        // Include the new ProjectileProperties source files
        PrivateIncludePaths.AddRange(new string[] { "LyokoWeapons/Private/Projectiles/Properties" });
    }
}
