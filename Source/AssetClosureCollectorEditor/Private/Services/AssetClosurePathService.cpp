#include "Services/AssetClosurePathService.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

FAssetClosureRootInput FAssetClosurePathService::ResolveRootInput(const FString& InInput)
{
	FAssetClosureRootInput Result;
	Result.OriginalInput = InInput;

	const FString TrimmedInput = InInput.TrimStartAndEnd();
	if (TrimmedInput.IsEmpty())
	{
		Result.ErrorMessage = TEXT("Input path is empty.");
		return Result;
	}

	FName PackageName;
	if (TrimmedInput.StartsWith(TEXT("/Game/")))
	{
		PackageName = FName(*NormalizeGamePath(TrimmedInput));
	}
	else if (!TryConvertDiskPathToPackageName(TrimmedInput, PackageName))
	{
		Result.ErrorMessage = TEXT("Input must be a /Game path or a file path inside the project's Content folder.");
		return Result;
	}

	FString ObjectPath;
	if (!TryBuildObjectPathFromPackageName(PackageName, ObjectPath))
	{
		Result.ErrorMessage = TEXT("Could not resolve an object path from the package name.");
		return Result;
	}

	Result.PackageName = PackageName;
	Result.ObjectPath = ObjectPath;
	Result.DiskPath = PackageNameToDiskPath(PackageName);
	Result.bResolved = true;
	return Result;
}

TArray<FAssetClosureRootInput> FAssetClosurePathService::ResolveRootInputs(const FString& InInputBlock)
{
	TArray<FAssetClosureRootInput> Results;
	TArray<FString> Lines;
	InInputBlock.ParseIntoArrayLines(Lines, true);

	for (FString& Line : Lines)
	{
		Line = Line.TrimStartAndEnd();
		if (Line.IsEmpty())
		{
			continue;
		}

		Results.Add(ResolveRootInput(Line));
	}

	return Results;
}

FString FAssetClosurePathService::PackageNameToDiskPath(const FName& PackageName)
{
	FString Filename;
	if (FPackageName::TryConvertLongPackageNameToFilename(PackageName.ToString(), Filename, FPackageName::GetAssetPackageExtension()))
	{
		return FPaths::ConvertRelativePathToFull(Filename);
	}

	return FString();
}

FName FAssetClosurePathService::BuildTargetPackageName(const FName& SourcePackageName, const FString& TargetRootPath)
{
	const FString NormalizedTargetRoot = NormalizeGamePath(TargetRootPath);
	const FString SourceString = SourcePackageName.ToString();
	const FString SourceRelativePath = SourceString.StartsWith(TEXT("/Game/")) ? SourceString.RightChop(6) : SourceString;
	return FName(*(NormalizedTargetRoot / SourceRelativePath));
}

bool FAssetClosurePathService::ValidateTargetRootPath(const FString& InTargetRootPath, FString& OutErrorMessage)
{
	const FString NormalizedPath = NormalizeGamePath(InTargetRootPath.TrimStartAndEnd());
	if (NormalizedPath.IsEmpty())
	{
		OutErrorMessage = TEXT("Target root path is empty.");
		return false;
	}

	if (!NormalizedPath.StartsWith(TEXT("/Game/")))
	{
		OutErrorMessage = TEXT("Target root path must stay under /Game/.");
		return false;
	}

	if (NormalizedPath.Equals(TEXT("/Game"), ESearchCase::IgnoreCase) || NormalizedPath.Equals(TEXT("/Game/"), ESearchCase::IgnoreCase))
	{
		OutErrorMessage = TEXT("Target root path cannot be the entire /Game root.");
		return false;
	}

	return true;
}

bool FAssetClosurePathService::TryConvertDiskPathToPackageName(const FString& DiskPath, FName& OutPackageName)
{
	const FString FullPath = FPaths::ConvertRelativePathToFull(DiskPath);
	if (!FPaths::FileExists(FullPath))
	{
		return false;
	}

	FString PackageName;
	if (!FPackageName::TryConvertFilenameToLongPackageName(FullPath, PackageName))
	{
		return false;
	}

	OutPackageName = FName(*PackageName);
	return true;
}

bool FAssetClosurePathService::TryBuildObjectPathFromPackageName(const FName& PackageName, FString& OutObjectPath)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	AssetRegistry.SearchAllAssets(true);

	const FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(FString::Printf(TEXT("%s.%s"), *PackageName.ToString(), *FPackageName::GetLongPackageAssetName(PackageName.ToString()))));
	if (AssetData.IsValid())
	{
		OutObjectPath = AssetData.GetSoftObjectPath().ToString();
		return true;
	}

	TArray<FAssetData> Assets;
	AssetRegistry.GetAssetsByPackageName(PackageName, Assets);
	if (Assets.Num() > 0)
	{
		OutObjectPath = Assets[0].GetSoftObjectPath().ToString();
		return true;
	}

	return false;
}

FString FAssetClosurePathService::NormalizeGamePath(const FString& InPath)
{
	FString Normalized = InPath;
	Normalized.ReplaceInline(TEXT("\\"), TEXT("/"));
	Normalized.RemoveFromEnd(TEXT("/"));
	return Normalized;
}
