// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameCombatEngineer : ModuleRules
{
    public GameCombatEngineer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
        PublicIncludePaths.AddRange(
    new string[] { "GameCombatEngineer/Public" }
);
        PrivateIncludePaths.AddRange(
            new string[] { "GameCombatEngineer/Private" }
        );
    }
}
