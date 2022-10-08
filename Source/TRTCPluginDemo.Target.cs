// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TRTCPluginDemoTarget : TargetRules
{
	public TRTCPluginDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "TRTCPluginDemo" });

		// TRTC Plugin
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			bOverrideBuildEnvironment = true;
			GlobalDefinitions.Add("FORCE_ANSI_ALLOCATOR=1");
		}
	}
}
