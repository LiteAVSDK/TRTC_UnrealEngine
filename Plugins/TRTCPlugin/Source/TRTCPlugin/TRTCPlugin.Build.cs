// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TRTCPlugin : ModuleRules
{
	public TRTCPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
                // "Public",
				// ... add public include paths required here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"TRTCSDK",

				// Test Only
				"UserSig",
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
			);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicAdditionalLibraries.Add("resolv");
			PublicFrameworks.AddRange(
			new string[] {
				"AppKit",
				"IOKit",
				"CoreVideo",
				"CFNetwork",
				"MetalKit",
				"OpenGl",
				"CoreGraphics",
				"Accelerate",
				"CoreFoundation",
				"SystemConfiguration",
				"AudioToolbox",
				"VideoToolbox",
				"CoreTelephony",
				"CoreWLAN",
				"AVFoundation",
				"CoreMedia",
				"CoreImage",
				"QuartzCore",
				"CoreAudio"
			});
		}

		bEnableUndefinedIdentifierWarnings = false;
	}
}
