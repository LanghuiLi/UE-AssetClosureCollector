#pragma once

#include "AssetClosureCollectorTypes.h"

class FAssetClosurePathService
{
public:
	static FAssetClosureRootInput ResolveRootInput(const FString& InInput);
	static TArray<FAssetClosureRootInput> ResolveRootInputs(const FString& InInputBlock);
	static FString PackageNameToDiskPath(const FName& PackageName);
	static FName BuildTargetPackageName(const FName& SourcePackageName, const FString& TargetRootPath);
	static bool ValidateTargetRootPath(const FString& InTargetRootPath, FString& OutErrorMessage);

private:
	static bool TryConvertDiskPathToPackageName(const FString& DiskPath, FName& OutPackageName);
	static bool TryBuildObjectPathFromPackageName(const FName& PackageName, FString& OutObjectPath);
	static FString NormalizeGamePath(const FString& InPath);
};
