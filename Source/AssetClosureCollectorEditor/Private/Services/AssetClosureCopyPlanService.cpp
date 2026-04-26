#include "Services/AssetClosureCopyPlanService.h"

#include "Services/AssetClosurePathService.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"

TArray<FAssetClosureCopyPlanItem> FAssetClosureCopyPlanService::BuildPlan(
	const FAssetClosureAnalysisResult& AnalysisResult,
	const FString& TargetRootPath)
{
	TArray<FAssetClosureCopyPlanItem> Plan;

	for (const TPair<FName, FAssetClosureNode>& Pair : AnalysisResult.Nodes)
	{
		const FAssetClosureNode& Node = Pair.Value;
		if (!Node.bExists || !Node.PackageName.ToString().StartsWith(TEXT("/Game/")))
		{
			continue;
		}

		FAssetClosureCopyPlanItem& Item = Plan.AddDefaulted_GetRef();
		Item.SourcePackageName = Node.PackageName;
		Item.SourceObjectPath = Node.ObjectPath;
		Item.SourceDiskPath = FAssetClosurePathService::PackageNameToDiskPath(Node.PackageName);
		Item.TargetPackageName = FAssetClosurePathService::BuildTargetPackageName(Node.PackageName, TargetRootPath);
		Item.TargetDiskPath = FAssetClosurePathService::PackageNameToDiskPath(Item.TargetPackageName);
		Item.bWillCopy = true;
		Item.Status = TEXT("Ready");

		if (Item.SourcePackageName == Item.TargetPackageName)
		{
			Item.bHasConflict = true;
			Item.bWillCopy = false;
			Item.Status = TEXT("Target path resolves to the source package.");
		}
	}

	Plan.Sort([](const FAssetClosureCopyPlanItem& A, const FAssetClosureCopyPlanItem& B)
	{
		return A.SourcePackageName.LexicalLess(B.SourcePackageName);
	});

	return Plan;
}
