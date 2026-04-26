#include "Services/AssetClosureDependencyService.h"

#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"

FAssetClosureAnalysisResult FAssetClosureDependencyService::Analyze(
	const TArray<FAssetClosureRootInput>& RootInputs,
	const FAssetClosureAnalyzeOptions& Options)
{
	FAssetClosureAnalysisResult Result;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	AssetRegistry.SearchAllAssets(true);

	TArray<FName> PendingPackages;
	TSet<FName> VisitedPackages;

	for (const FAssetClosureRootInput& RootInput : RootInputs)
	{
		if (!RootInput.bResolved)
		{
			Result.Errors.Add(FString::Printf(TEXT("Skipped unresolved input: %s"), *RootInput.OriginalInput));
			continue;
		}

		FAssetClosureNode& RootNode = Result.Nodes.FindOrAdd(RootInput.PackageName);
		RootNode.PackageName = RootInput.PackageName;
		RootNode.ObjectPath = RootInput.ObjectPath;
		RootNode.AssetName = FPackageName::GetLongPackageAssetName(RootInput.PackageName.ToString());
		RootNode.bIsRoot = true;
		RootNode.bExists = true;
		AddNodeMetadata(AssetRegistry, RootNode);

		PendingPackages.Add(RootInput.PackageName);
	}

	while (PendingPackages.Num() > 0)
	{
		const FName CurrentPackage = PendingPackages.Pop(EAllowShrinking::No);
		if (VisitedPackages.Contains(CurrentPackage))
		{
			continue;
		}

		VisitedPackages.Add(CurrentPackage);

		const TArray<TPair<FName, EAssetClosureDependencyType>> Dependencies = GetDependenciesForPackage(
			AssetRegistry,
			CurrentPackage,
			Options);

		for (const TPair<FName, EAssetClosureDependencyType>& Dependency : Dependencies)
		{
			FAssetClosureNode& Node = Result.Nodes.FindOrAdd(Dependency.Key);
			Node.PackageName = Dependency.Key;
			Node.AssetName = FPackageName::GetLongPackageAssetName(Dependency.Key.ToString());
			Node.DependencyTypes.Add(Dependency.Value);
			Node.ReferencedBy.AddUnique(CurrentPackage);
			AddNodeMetadata(AssetRegistry, Node);

			if (Options.bRecursive)
			{
				PendingPackages.Add(Dependency.Key);
			}
		}
	}

	return Result;
}

TArray<TPair<FName, EAssetClosureDependencyType>> FAssetClosureDependencyService::GetDependenciesForPackage(
	IAssetRegistry& AssetRegistry,
	const FName& PackageName,
	const FAssetClosureAnalyzeOptions& Options)
{
	TArray<TPair<FName, EAssetClosureDependencyType>> Result;

	auto AppendDependencies =
		[&AssetRegistry, &PackageName, &Result](UE::AssetRegistry::EDependencyCategory Category, UE::AssetRegistry::EDependencyQuery Query, EAssetClosureDependencyType DependencyType)
		{
			TArray<FName> Dependencies;
			AssetRegistry.GetDependencies(PackageName, Dependencies, Category, Query);
			for (const FName& Dependency : Dependencies)
			{
				Result.Add(TPair<FName, EAssetClosureDependencyType>(Dependency, DependencyType));
			}
		};

	if (Options.bIncludeHard)
	{
		AppendDependencies(
			UE::AssetRegistry::EDependencyCategory::Package,
			UE::AssetRegistry::EDependencyQuery::Hard,
			EAssetClosureDependencyType::Hard);
	}

	if (Options.bIncludeSoft)
	{
		AppendDependencies(
			UE::AssetRegistry::EDependencyCategory::Package,
			UE::AssetRegistry::EDependencyQuery::Soft,
			EAssetClosureDependencyType::Soft);
	}

	if (Options.bIncludeManage)
	{
		AppendDependencies(
			UE::AssetRegistry::EDependencyCategory::Manage,
			UE::AssetRegistry::EDependencyQuery::NoRequirements,
			EAssetClosureDependencyType::Manage);
	}

	if (Options.bIncludeSearchableName)
	{
		AppendDependencies(
			UE::AssetRegistry::EDependencyCategory::SearchableName,
			UE::AssetRegistry::EDependencyQuery::NoRequirements,
			EAssetClosureDependencyType::SearchableName);
	}

	return Result;
}

void FAssetClosureDependencyService::AddNodeMetadata(IAssetRegistry& AssetRegistry, FAssetClosureNode& Node)
{
	TArray<FAssetData> Assets;
	AssetRegistry.GetAssetsByPackageName(Node.PackageName, Assets);
	if (Assets.Num() == 0)
	{
		Node.bExists = false;
		return;
	}

	const FAssetData& AssetData = Assets[0];
	Node.ObjectPath = AssetData.GetSoftObjectPath().ToString();
	Node.AssetName = AssetData.AssetName.ToString();
	Node.AssetClass = AssetData.AssetClassPath.ToString();
	Node.bExists = true;
}
