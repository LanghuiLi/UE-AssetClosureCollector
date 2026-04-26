#include "Services/AssetClosureExecutionService.h"

#include "EditorAssetLibrary.h"

TArray<FAssetClosureCleanupPlanItem> FAssetClosureExecutionService::BuildCleanupPlan(
	const TArray<FAssetClosureCopyPlanItem>& CopyPlan,
	const FString& TargetRootPath)
{
	TArray<FAssetClosureCleanupPlanItem> CleanupPlan;
	TSet<FString> ExpectedPackages;

	for (const FAssetClosureCopyPlanItem& Item : CopyPlan)
	{
		if (Item.bWillCopy && !Item.TargetPackageName.IsNone())
		{
			ExpectedPackages.Add(Item.TargetPackageName.ToString());
		}
	}

	const TArray<FString> ExistingAssets = UEditorAssetLibrary::ListAssets(TargetRootPath, true, false);
	for (const FString& AssetPath : ExistingAssets)
	{
		const FString CandidatePackage = AssetPath.Contains(TEXT(".")) ? AssetPath.Left(AssetPath.Find(TEXT("."))) : AssetPath;
		if (!ExpectedPackages.Contains(CandidatePackage))
		{
			FAssetClosureCleanupPlanItem& Item = CleanupPlan.AddDefaulted_GetRef();
			Item.TargetPackageName = FName(*CandidatePackage);
			Item.bWillDelete = true;
			Item.Status = TEXT("Will delete");
		}
	}

	CleanupPlan.Sort([](const FAssetClosureCleanupPlanItem& A, const FAssetClosureCleanupPlanItem& B)
	{
		return A.TargetPackageName.LexicalLess(B.TargetPackageName);
	});

	return CleanupPlan;
}

FAssetClosureExecutionResult FAssetClosureExecutionService::ExecuteCopyPlan(
	const TArray<FAssetClosureCopyPlanItem>& CopyPlan)
{
	FAssetClosureExecutionResult Result;

	for (const FAssetClosureCopyPlanItem& Item : CopyPlan)
	{
		if (!Item.bWillCopy || Item.bHasConflict)
		{
			continue;
		}

		if (UEditorAssetLibrary::DoesAssetExist(Item.TargetPackageName.ToString()))
		{
			UEditorAssetLibrary::DeleteAsset(Item.TargetPackageName.ToString());
		}

		UObject* DuplicatedAsset = UEditorAssetLibrary::DuplicateAsset(
			Item.SourcePackageName.ToString(),
			Item.TargetPackageName.ToString());
		const bool bDuplicated = (DuplicatedAsset != nullptr);

		if (bDuplicated)
		{
			++Result.SucceededCount;
			Result.Infos.Add(FString::Printf(TEXT("Copied %s -> %s"), *Item.SourcePackageName.ToString(), *Item.TargetPackageName.ToString()));
		}
		else
		{
			++Result.FailedCount;
			Result.Errors.Add(FString::Printf(TEXT("Failed to copy %s -> %s"), *Item.SourcePackageName.ToString(), *Item.TargetPackageName.ToString()));
		}
	}

	return Result;
}

FAssetClosureExecutionResult FAssetClosureExecutionService::ExecuteCleanupPlan(
	const TArray<FAssetClosureCleanupPlanItem>& CleanupPlan)
{
	FAssetClosureExecutionResult Result;

	for (const FAssetClosureCleanupPlanItem& Item : CleanupPlan)
	{
		if (!Item.bWillDelete)
		{
			continue;
		}

		const bool bDeleted = UEditorAssetLibrary::DeleteAsset(Item.TargetPackageName.ToString());
		if (bDeleted)
		{
			++Result.SucceededCount;
			Result.Infos.Add(FString::Printf(TEXT("Deleted %s"), *Item.TargetPackageName.ToString()));
		}
		else
		{
			++Result.FailedCount;
			Result.Errors.Add(FString::Printf(TEXT("Failed to delete %s"), *Item.TargetPackageName.ToString()));
		}
	}

	return Result;
}
