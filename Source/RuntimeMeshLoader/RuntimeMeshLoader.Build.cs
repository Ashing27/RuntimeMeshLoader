// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class RuntimeMeshLoader : ModuleRules
{
	public RuntimeMeshLoader(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
			/*Here's some notice for helping use these config:
			 * PublicDefinitions			:	The definitions you wanna add in build your plugin.
			 * PublicIncludePaths			:	Additional include path for your plugin.
			 * PublicAdditionalLibraries	:	.lib file paths.
			 * PublicDelayLoadDlls			:	Load dlls in a delay way, Engine will only load dll 1 time.
			 * RuntimeDependencies			:   Dlls listed here will be packaged with .exe.
			 */

			/* I leave a config here, can make sure this dll file could be packaged as a depency file.
			 * it can be used in function: 
			 * StartupModule and ShutdownModule
			 */
			///RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Binaries/Win64", "assimp-vc142-mt.dll"));

			/* Following config will made this dll file copied to the path where the .exe is.
			 * it can let the windows find it easier instead of add it into register.
			 */
			RuntimeDependencies.Add("$(TargetOutputDir)/assimp-vc142-mt.dll", Path.Combine(PluginDirectory, "Binaries/Win64", "assimp-vc142-mt.dll"));
			PublicIncludePaths.Add(Path.Combine(PluginDirectory, "Source/assimp/include"));
			PrivateIncludePaths.Add(Path.Combine(PluginDirectory, "Source/assimp/include"));
			PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Binaries/Win64", "assimp-vc142-mt.lib"));
		}
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"RHI",
				"RenderCore",
				"ProceduralMeshComponent",
				"Projects"
				// ... add other public dependencies that you statically link with here ...
			}
			);
	}
}
