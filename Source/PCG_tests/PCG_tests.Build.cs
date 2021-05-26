// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PCG_tests : ModuleRules {
	public PCG_tests (ReadOnlyTargetRules Target) : base (Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange (new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PublicDependencyModuleNames.Add ("PCGToolbox");
		PublicDependencyModuleNames.Add ("Landscape");
		PublicDependencyModuleNames.Add ("LandscapeEditor");

		PrivateDependencyModuleNames.AddRange (new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}