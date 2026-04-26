#pragma once

#include "CoreMinimal.h"

enum class EAssetClosureDependencyType : uint8
{
	Hard,
	Soft,
	Manage,
	SearchableName
};

struct FAssetClosureRootInput
{
	FString OriginalInput;
	FName PackageName;
	FString ObjectPath;
	FString DiskPath;
	bool bResolved = false;
	FString ErrorMessage;
};

struct FAssetClosureNode
{
	FName PackageName;
	FString ObjectPath;
	FString AssetName;
	FString AssetClass;
	TSet<EAssetClosureDependencyType> DependencyTypes;
	TArray<FName> ReferencedBy;
	bool bExists = false;
	bool bIsRoot = false;
};

struct FAssetClosureAnalyzeOptions
{
	bool bIncludeHard = true;
	bool bIncludeSoft = true;
	bool bIncludeManage = false;
	bool bIncludeSearchableName = false;
	bool bRecursive = true;
};

struct FAssetClosureAnalysisResult
{
	TMap<FName, FAssetClosureNode> Nodes;
	TArray<FString> Warnings;
	TArray<FString> Errors;
};

struct FAssetClosureCopyPlanItem
{
	FName SourcePackageName;
	FString SourceObjectPath;
	FString SourceDiskPath;
	FName TargetPackageName;
	FString TargetDiskPath;
	bool bWillCopy = false;
	bool bHasConflict = false;
	FString Status;
};

struct FAssetClosureCleanupPlanItem
{
	FName TargetPackageName;
	bool bWillDelete = false;
	FString Status;
};

struct FAssetClosureExecutionResult
{
	int32 SucceededCount = 0;
	int32 FailedCount = 0;
	TArray<FString> Infos;
	TArray<FString> Errors;
};
