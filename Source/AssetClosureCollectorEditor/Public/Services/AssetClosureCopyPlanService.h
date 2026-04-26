#pragma once

#include "AssetClosureCollectorTypes.h"

class FAssetClosureCopyPlanService
{
public:
	static TArray<FAssetClosureCopyPlanItem> BuildPlan(
		const FAssetClosureAnalysisResult& AnalysisResult,
		const FString& TargetRootPath);
};
