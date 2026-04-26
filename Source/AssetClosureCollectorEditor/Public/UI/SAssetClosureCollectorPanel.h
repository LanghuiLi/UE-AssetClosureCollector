#pragma once

#include "AssetClosureCollectorTypes.h"
#include "Widgets/SCompoundWidget.h"

class SCheckBox;
class SEditableTextBox;
class SMultiLineEditableTextBox;

enum class EAssetClosureCollectorLanguage : uint8
{
	English,
	Chinese
};

class SAssetClosureCollectorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetClosureCollectorPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnAnalyzeClicked();
	FReply OnBrowseLocalFileClicked();
	FReply OnBrowseContentAssetClicked();
	FReply OnSwitchToEnglishClicked();
	FReply OnSwitchToChineseClicked();
	FReply OnShowUsageGuideClicked();
	FReply OnCopyAssetsClicked();
	FReply OnCopyAndCleanClicked();
	void RefreshOutput();
	void AppendSourceInput(const FString& NewSourceInput);
	bool PrepareAnalysis(TArray<FAssetClosureRootInput>& OutRootInputs);
	bool PrepareExecution(bool bWithCleanup, bool& bOutDryRun);
	bool ConfirmExecution(bool bWithCleanup) const;
	FText GetLocalizedText(const TCHAR* EnglishText, const TCHAR* ChineseText) const;
	FText GetUsageGuideText() const;
	FSlateColor GetEnglishButtonColor() const;
	FSlateColor GetChineseButtonColor() const;

	ECheckBoxState GetIncludeHardState() const;
	void OnIncludeHardChanged(ECheckBoxState NewState);
	ECheckBoxState GetIncludeSoftState() const;
	void OnIncludeSoftChanged(ECheckBoxState NewState);
	ECheckBoxState GetIncludeManageState() const;
	void OnIncludeManageChanged(ECheckBoxState NewState);
	ECheckBoxState GetRecursiveState() const;
	void OnRecursiveChanged(ECheckBoxState NewState);
	ECheckBoxState GetDryRunState() const;
	void OnDryRunChanged(ECheckBoxState NewState);

	FText GetSummaryText() const;
	FText GetResultsText() const;

	EAssetClosureCollectorLanguage CurrentLanguage = EAssetClosureCollectorLanguage::English;
	bool bDryRun = true;
	FAssetClosureAnalyzeOptions AnalyzeOptions;
	FAssetClosureAnalysisResult AnalysisResult;
	TArray<FAssetClosureCopyPlanItem> CopyPlan;
	TArray<FAssetClosureCleanupPlanItem> CleanupPlan;
	FAssetClosureExecutionResult LastExecutionResult;
	FString SourceInput;
	FString TargetRootPath;
	TSharedPtr<SMultiLineEditableTextBox> SourceInputTextBox;
	TSharedPtr<SEditableTextBox> TargetRootPathTextBox;
};
