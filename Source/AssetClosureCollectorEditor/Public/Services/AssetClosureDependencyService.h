#pragma once

#include "AssetClosureCollectorTypes.h"

class FAssetClosureDependencyService
{
public:
	static FAssetClosureAnalysisResult Analyze(
		const TArray<FAssetClosureRootInput>& RootInputs,
		const FAssetClosureAnalyzeOptions& Options);

private:
	static TArray<TPair<FName, EAssetClosureDependencyType>> GetDependenciesForPackage(
		class IAssetRegistry& AssetRegistry,
		const FName& PackageName,
		const FAssetClosureAnalyzeOptions& Options);

	static void AddNodeMetadata(
		class IAssetRegistry& AssetRegistry,
		FAssetClosureNode& Node);
};
