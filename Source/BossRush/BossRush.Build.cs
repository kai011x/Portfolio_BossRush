// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BossRush : ModuleRules
{
	public BossRush(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
																	"GameplayAbilities", "GameplayTasks", "GameplayTags", "CableComponent", "UMG", "Niagara", "AIModule",
																	"StateTreeModule", "GameplayStateTreeModule", "StructUtils"});

		PrivateDependencyModuleNames.AddRange(new string[] {});

		PublicIncludePaths.Add(ModuleDirectory);
	}
}


