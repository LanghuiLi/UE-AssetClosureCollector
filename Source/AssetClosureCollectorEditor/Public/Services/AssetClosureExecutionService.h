#pragma once

#include "AssetClosureCollectorTypes.h"

class FAssetClosureExecutionService
{
public:
	static TArray<FAssetClosureCleanupPlanItem> BuildCleanupPlan(
		const TArray<FAssetClosureCopyPlanItem>& CopyPlan,
		const FString& TargetRootPath);

	static FAssetClosureExecutionResult ExecuteCopyPlan(
		const TArray<FAssetClosureCopyPlanItem>& CopyPlan);

	static FAssetClosureExecutionResult ExecuteCleanupPlan(
		const TArray<FAssetClosureCleanupPlanItem>& CleanupPlan);
};
