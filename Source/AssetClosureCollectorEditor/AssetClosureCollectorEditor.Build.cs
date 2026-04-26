using UnrealBuildTool;

public class AssetClosureCollectorEditor : ModuleRules
{
	public AssetClosureCollectorEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"AssetRegistry",
				"AssetTools",
				"ContentBrowser",
				"DesktopPlatform",
				"EditorScriptingUtilities",
				"EditorFramework",
				"EditorStyle",
				"InputCore",
				"Projects",
				"ToolMenus",
				"UnrealEd"
			}
		);
	}
}
