#include "Modules/ModuleManager.h"

#include "Framework/Docking/TabManager.h"
#include "ToolMenus.h"
#include "UI/SAssetClosureCollectorPanel.h"
#include "Widgets/Docking/SDockTab.h"

namespace
{
	static const FName AssetClosureCollectorTabName(TEXT("AssetClosureCollector"));
}

class FAssetClosureCollectorEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
			AssetClosureCollectorTabName,
			FOnSpawnTab::CreateRaw(this, &FAssetClosureCollectorEditorModule::SpawnCollectorTab))
			.SetDisplayName(FText::FromString(TEXT("Asset Closure Collector")))
			.SetMenuType(ETabSpawnerMenuType::Hidden);

		UToolMenus::RegisterStartupCallback(
			FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetClosureCollectorEditorModule::RegisterMenus));
	}

	virtual void ShutdownModule() override
	{
		if (UObjectInitialized() && UToolMenus::TryGet())
		{
			UToolMenus::UnRegisterStartupCallback(this);
			UToolMenus::UnregisterOwner(this);
		}

		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AssetClosureCollectorTabName);
	}

private:
	TSharedRef<SDockTab> SpawnCollectorTab(const FSpawnTabArgs& SpawnTabArgs)
	{
		return SNew(SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				SNew(SAssetClosureCollectorPanel)
			];
	}

	void RegisterMenus()
	{
		FToolMenuOwnerScoped OwnerScoped(this);

		UToolMenu* WindowMenu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Window"));
		FToolMenuSection& Section = WindowMenu->FindOrAddSection(TEXT("WindowLayout"));
		Section.AddMenuEntry(
			TEXT("OpenAssetClosureCollector"),
			FText::FromString(TEXT("Asset Closure Collector")),
			FText::FromString(TEXT("Open the asset closure collector tool.")),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FAssetClosureCollectorEditorModule::OpenCollectorTab)));
	}

	void OpenCollectorTab()
	{
		FGlobalTabmanager::Get()->TryInvokeTab(AssetClosureCollectorTabName);
	}
};

IMPLEMENT_MODULE(FAssetClosureCollectorEditorModule, AssetClosureCollectorEditor)
