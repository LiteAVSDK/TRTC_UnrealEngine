// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TRTCPluginDemo : ModuleRules
{
	public TRTCPluginDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });

		// TRTC Plugin Requirement
		PrivateDependencyModuleNames.AddRange(new string[] { "TRTCPlugin" });
		bEnableUndefinedIdentifierWarnings = false;
	}
}
