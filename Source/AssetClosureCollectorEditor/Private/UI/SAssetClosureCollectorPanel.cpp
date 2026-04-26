#include "UI/SAssetClosureCollectorPanel.h"

#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "Dialogs/Dialogs.h"
#include "Framework/Application/SlateApplication.h"
#include "IContentBrowserSingleton.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "Services/AssetClosureCopyPlanService.h"
#include "Services/AssetClosureDependencyService.h"
#include "Services/AssetClosureExecutionService.h"
#include "Services/AssetClosurePathService.h"
#include "Styling/AppStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SWindow.h"
#include "Widgets/Text/STextBlock.h"

void SAssetClosureCollectorPanel::Construct(const FArguments& InArgs)
{
	TargetRootPath = TEXT("/Game/CollectedAssets");

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text_Lambda([this]() { return GetLocalizedText(TEXT("Language"), TEXT("\u8BED\u8A00")); })
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				[
					SNew(SButton)
					.Text_Lambda([this]() { return GetLocalizedText(TEXT("Usage Guide"), TEXT("\u4F7F\u7528\u8BF4\u660E")); })
					.OnClicked(this, &SAssetClosureCollectorPanel::OnShowUsageGuideClicked)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonColorAndOpacity(this, &SAssetClosureCollectorPanel::GetEnglishButtonColor)
					.Text(FText::FromString(TEXT("English")))
					.OnClicked(this, &SAssetClosureCollectorPanel::OnSwitchToEnglishClicked)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.ButtonColorAndOpacity(this, &SAssetClosureCollectorPanel::GetChineseButtonColor)
					.Text(FText::FromString(TEXT("\u4E2D\u6587")))
					.OnClicked(this, &SAssetClosureCollectorPanel::OnSwitchToChineseClicked)
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f, 0.0f, 8.0f, 4.0f)
			[
				SNew(SSeparator)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return GetLocalizedText(TEXT("Source Asset Paths"), TEXT("\u6E90\u8D44\u6E90\u8DEF\u5F84")); })
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f, 0.0f, 8.0f, 2.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return GetLocalizedText(TEXT("One asset per line"), TEXT("\u6BCF\u884C\u4E00\u4E2A\u8D44\u6E90")); })
				.ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f)))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SBox)
					.MinDesiredHeight(86.0f)
					[
						SAssignNew(SourceInputTextBox, SMultiLineEditableTextBox)
						.HintText_Lambda([this]() { return GetLocalizedText(TEXT("/Game/... or Content file path"), TEXT("/Game/... \u6216 Content \u4E0B\u7684\u6587\u4EF6\u8DEF\u5F84")); })
						.OnTextChanged_Lambda([this](const FText& NewText) { SourceInput = NewText.ToString(); })
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 0.0f, 0.0f, 8.0f)
					[
						SNew(SButton)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Add File"), TEXT("\u6DFB\u52A0\u6587\u4EF6")); })
						.OnClicked(this, &SAssetClosureCollectorPanel::OnBrowseLocalFileClicked)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SButton)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Add Asset"), TEXT("\u6DFB\u52A0\u8D44\u6E90")); })
						.OnClicked(this, &SAssetClosureCollectorPanel::OnBrowseContentAssetClicked)
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return GetLocalizedText(TEXT("Target Root Path"), TEXT("\u76EE\u6807\u6839\u8DEF\u5F84")); })
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SAssignNew(TargetRootPathTextBox, SEditableTextBox)
				.Text(FText::FromString(TargetRootPath))
				.OnTextChanged_Lambda([this](const FText& NewText) { TargetRootPath = NewText.ToString(); })
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(FMargin(8.0f, 4.0f))
				+ SUniformGridPanel::Slot(0, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SAssetClosureCollectorPanel::GetIncludeHardState)
					.OnCheckStateChanged(this, &SAssetClosureCollectorPanel::OnIncludeHardChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Include Hard"), TEXT("\u5305\u542B\u786C\u4F9D\u8D56")); })
					]
				]
				+ SUniformGridPanel::Slot(1, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SAssetClosureCollectorPanel::GetIncludeSoftState)
					.OnCheckStateChanged(this, &SAssetClosureCollectorPanel::OnIncludeSoftChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Include Soft"), TEXT("\u5305\u542B\u8F6F\u4F9D\u8D56")); })
					]
				]
				+ SUniformGridPanel::Slot(0, 1)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SAssetClosureCollectorPanel::GetIncludeManageState)
					.OnCheckStateChanged(this, &SAssetClosureCollectorPanel::OnIncludeManageChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Include Manage"), TEXT("\u5305\u542B\u7BA1\u7406\u4F9D\u8D56")); })
					]
				]
				+ SUniformGridPanel::Slot(1, 1)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SAssetClosureCollectorPanel::GetRecursiveState)
					.OnCheckStateChanged(this, &SAssetClosureCollectorPanel::OnRecursiveChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Recursive"), TEXT("\u9012\u5F52\u5C55\u5F00")); })
					]
				]
				+ SUniformGridPanel::Slot(0, 2)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SAssetClosureCollectorPanel::GetDryRunState)
					.OnCheckStateChanged(this, &SAssetClosureCollectorPanel::OnDryRunChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return GetLocalizedText(TEXT("Dry Run"), TEXT("\u6F14\u7EC3\u6A21\u5F0F\uff08\u4E0D\u5B9E\u9645\u590D\u5236/\u5220\u9664\uff09")); })
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SButton)
					.Text_Lambda([this]() { return GetLocalizedText(TEXT("Analyze Closure"), TEXT("\u5206\u6790\u4F9D\u8D56\u95ED\u5305")); })
					.OnClicked(this, &SAssetClosureCollectorPanel::OnAnalyzeClicked)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SButton)
					.Text_Lambda([this]() { return GetLocalizedText(TEXT("Copy Assets"), TEXT("\u590D\u5236\u8D44\u6E90")); })
					.OnClicked(this, &SAssetClosureCollectorPanel::OnCopyAssetsClicked)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SButton)
					.Text_Lambda([this]() { return GetLocalizedText(TEXT("Copy + Clean Target"), TEXT("\u590D\u5236\u5E76\u6E05\u7406\u76EE\u6807\u76EE\u5F55")); })
					.OnClicked(this, &SAssetClosureCollectorPanel::OnCopyAndCleanClicked)
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f)
			[
				SNew(STextBlock)
				.Text(this, &SAssetClosureCollectorPanel::GetSummaryText)
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(8.0f)
			[
				SNew(SMultiLineEditableTextBox)
				.IsReadOnly(true)
				.Text(this, &SAssetClosureCollectorPanel::GetResultsText)
			]
		]
	];
}

FReply SAssetClosureCollectorPanel::OnAnalyzeClicked()
{
	TArray<FAssetClosureRootInput> RootInputs;
	if (!PrepareAnalysis(RootInputs))
	{
		return FReply::Handled();
	}

	AnalysisResult = FAssetClosureDependencyService::Analyze(RootInputs, AnalyzeOptions);
	CopyPlan = FAssetClosureCopyPlanService::BuildPlan(AnalysisResult, TargetRootPath);
	CleanupPlan = FAssetClosureExecutionService::BuildCleanupPlan(CopyPlan, TargetRootPath);
	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnBrowseLocalFileClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform == nullptr)
	{
		AnalysisResult.Errors.Add(GetLocalizedText(TEXT("Desktop platform service is unavailable."), TEXT("\u684C\u9762\u5E73\u53F0\u670D\u52A1\u4E0D\u53EF\u7528\u3002")).ToString());
		return FReply::Handled();
	}

	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(AsShared());
	TArray<FString> SelectedFiles;
	const bool bOpened = DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		GetLocalizedText(TEXT("Choose asset files"), TEXT("\u9009\u62E9\u8D44\u6E90\u6587\u4EF6")).ToString(),
		FPaths::ProjectContentDir(),
		TEXT(""),
		TEXT("Unreal Assets (*.uasset)|*.uasset"),
		EFileDialogFlags::Multiple,
		SelectedFiles);

	if (bOpened)
	{
		for (const FString& SelectedFile : SelectedFiles)
		{
			AppendSourceInput(SelectedFile);
		}
	}

	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnBrowseContentAssetClicked()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	FOpenAssetDialogConfig Config;
	Config.DialogTitleOverride = GetLocalizedText(TEXT("Choose assets"), TEXT("\u9009\u62E9\u8D44\u6E90"));
	Config.bAllowMultipleSelection = true;
	Config.DefaultPath = TEXT("/Game");

	const TArray<FAssetData> SelectedAssets = ContentBrowserModule.Get().CreateModalOpenAssetDialog(Config);
	for (const FAssetData& SelectedAsset : SelectedAssets)
	{
		AppendSourceInput(SelectedAsset.PackageName.ToString());
	}

	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnSwitchToEnglishClicked()
{
	CurrentLanguage = EAssetClosureCollectorLanguage::English;
	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnSwitchToChineseClicked()
{
	CurrentLanguage = EAssetClosureCollectorLanguage::Chinese;
	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnShowUsageGuideClicked()
{
	TSharedRef<SWindow> UsageGuideWindow = SNew(SWindow)
		.Title(GetLocalizedText(TEXT("Asset Closure Collector Guide"), TEXT("Asset Closure Collector \u4F7F\u7528\u8BF4\u660E")))
		.ClientSize(FVector2D(720.0f, 560.0f))
		.SupportsMinimize(false)
		.SupportsMaximize(false);

	UsageGuideWindow->SetContent(
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(12.0f)
		[
			SNew(SMultiLineEditableTextBox)
			.IsReadOnly(true)
			.AutoWrapText(true)
			.Text(this, &SAssetClosureCollectorPanel::GetUsageGuideText)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(12.0f)
		[
			SNew(SButton)
			.Text(GetLocalizedText(TEXT("Close"), TEXT("\u5173\u95ED")))
			.OnClicked_Lambda([UsageGuideWindow]()
			{
				UsageGuideWindow->RequestDestroyWindow();
				return FReply::Handled();
			})
		]);

	FSlateApplication::Get().AddModalWindow(
		UsageGuideWindow,
		FSlateApplication::Get().FindWidgetWindow(AsShared()),
		false);

	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnCopyAssetsClicked()
{
	bool bPreviewOnly = false;
	if (!PrepareExecution(false, bPreviewOnly))
	{
		return FReply::Handled();
	}

	if (bPreviewOnly)
	{
		return FReply::Handled();
	}

	LastExecutionResult = FAssetClosureExecutionService::ExecuteCopyPlan(CopyPlan);
	return FReply::Handled();
}

FReply SAssetClosureCollectorPanel::OnCopyAndCleanClicked()
{
	bool bPreviewOnly = false;
	if (!PrepareExecution(true, bPreviewOnly))
	{
		return FReply::Handled();
	}

	if (bPreviewOnly)
	{
		return FReply::Handled();
	}

	LastExecutionResult = FAssetClosureExecutionService::ExecuteCopyPlan(CopyPlan);
	const FAssetClosureExecutionResult CleanupResult = FAssetClosureExecutionService::ExecuteCleanupPlan(CleanupPlan);
	LastExecutionResult.SucceededCount += CleanupResult.SucceededCount;
	LastExecutionResult.FailedCount += CleanupResult.FailedCount;
	LastExecutionResult.Infos.Append(CleanupResult.Infos);
	LastExecutionResult.Errors.Append(CleanupResult.Errors);
	return FReply::Handled();
}

void SAssetClosureCollectorPanel::RefreshOutput()
{
}

void SAssetClosureCollectorPanel::AppendSourceInput(const FString& NewSourceInput)
{
	const FString TrimmedInput = NewSourceInput.TrimStartAndEnd();
	if (TrimmedInput.IsEmpty())
	{
		return;
	}

	if (SourceInput.IsEmpty())
	{
		SourceInput = TrimmedInput;
	}
	else
	{
		TArray<FString> ExistingLines;
		SourceInput.ParseIntoArrayLines(ExistingLines, true);
		if (!ExistingLines.Contains(TrimmedInput))
		{
			SourceInput += LINE_TERMINATOR;
			SourceInput += TrimmedInput;
		}
	}

	if (SourceInputTextBox.IsValid())
	{
		SourceInputTextBox->SetText(FText::FromString(SourceInput));
	}
}

bool SAssetClosureCollectorPanel::PrepareAnalysis(TArray<FAssetClosureRootInput>& OutRootInputs)
{
	AnalysisResult = FAssetClosureAnalysisResult();
	CopyPlan.Reset();
	CleanupPlan.Reset();
	LastExecutionResult = FAssetClosureExecutionResult();

	FString TargetPathError;
	if (!FAssetClosurePathService::ValidateTargetRootPath(TargetRootPath, TargetPathError))
	{
		AnalysisResult.Errors.Add(TargetPathError);
		return false;
	}

	OutRootInputs = FAssetClosurePathService::ResolveRootInputs(SourceInput);
	if (OutRootInputs.Num() == 0)
	{
		AnalysisResult.Errors.Add(GetLocalizedText(TEXT("Add at least one source asset."), TEXT("\u8BF7\u81F3\u5C11\u6DFB\u52A0\u4E00\u4E2A\u6E90\u8D44\u6E90\u3002")).ToString());
		return false;
	}

	return true;
}

bool SAssetClosureCollectorPanel::PrepareExecution(bool bWithCleanup, bool& bOutDryRun)
{
	bOutDryRun = false;
	LastExecutionResult = FAssetClosureExecutionResult();

	if (CopyPlan.Num() == 0)
	{
		LastExecutionResult.Errors.Add(GetLocalizedText(TEXT("Run Analyze Closure before executing."), TEXT("\u8BF7\u5148\u6267\u884C\u4F9D\u8D56\u95ED\u5305\u5206\u6790\u3002")).ToString());
		return false;
	}

	FString TargetPathError;
	if (!FAssetClosurePathService::ValidateTargetRootPath(TargetRootPath, TargetPathError))
	{
		LastExecutionResult.Errors.Add(TargetPathError);
		return false;
	}

	if (bDryRun)
	{
		bOutDryRun = true;
		if (CurrentLanguage == EAssetClosureCollectorLanguage::Chinese)
		{
			LastExecutionResult.Infos.Add(FString::Printf(
				TEXT("Dry Run: \u5C06\u590D\u5236 %d \u4E2A\u8D44\u6E90\uFF0C\u6E05\u7406 %d \u4E2A\u76EE\u6807\u8D44\u6E90"),
				CopyPlan.Num(),
				bWithCleanup ? CleanupPlan.Num() : 0));
		}
		else
		{
			LastExecutionResult.Infos.Add(FString::Printf(
				TEXT("Dry Run: would copy %d assets and clean %d target assets"),
				CopyPlan.Num(),
				bWithCleanup ? CleanupPlan.Num() : 0));
		}
		return true;
	}

	if (!ConfirmExecution(bWithCleanup))
	{
		LastExecutionResult.Infos.Add(GetLocalizedText(TEXT("Execution canceled by user."), TEXT("\u7528\u6237\u5DF2\u53D6\u6D88\u6267\u884C\u3002")).ToString());
		return false;
	}

	return true;
}

bool SAssetClosureCollectorPanel::ConfirmExecution(bool bWithCleanup) const
{
	const FText Message = bWithCleanup
		? GetLocalizedText(
			*FString::Printf(TEXT("Copy %d assets to %s and clean %d assets from the target folder?"), CopyPlan.Num(), *TargetRootPath, CleanupPlan.Num()),
			*FString::Printf(TEXT("\u8981\u590D\u5236 %d \u4E2A\u8D44\u6E90\u5230 %s\uff0c\u5E76\u6E05\u7406\u76EE\u6807\u76EE\u5F55\u4E2D\u7684 %d \u4E2A\u8D44\u6E90\u5417\uff1F"), CopyPlan.Num(), *TargetRootPath, CleanupPlan.Num()))
		: GetLocalizedText(
			*FString::Printf(TEXT("Copy %d assets to %s?"), CopyPlan.Num(), *TargetRootPath),
			*FString::Printf(TEXT("\u8981\u590D\u5236 %d \u4E2A\u8D44\u6E90\u5230 %s \u5417\uff1F"), CopyPlan.Num(), *TargetRootPath));

	return FMessageDialog::Open(EAppMsgType::YesNo, Message) == EAppReturnType::Yes;
}

FText SAssetClosureCollectorPanel::GetLocalizedText(const TCHAR* EnglishText, const TCHAR* ChineseText) const
{
	return FText::FromString(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? ChineseText : EnglishText);
}

FText SAssetClosureCollectorPanel::GetUsageGuideText() const
{
	if (CurrentLanguage == EAssetClosureCollectorLanguage::Chinese)
	{
		return FText::FromString(
			TEXT("1. \u5728\u9876\u90E8\u9009\u62E9\u754C\u9762\u8BED\u8A00\uFF0C\u53EF\u968F\u65F6\u5207\u6362\u4E2D\u6587\u6216\u82F1\u6587\u3002\n\n")
			TEXT("2. \u6E90\u8D44\u6E90\u8F93\u5165\u533A\u652F\u6301\u591A\u8D44\u6E90\u3002\u6BCF\u884C\u8F93\u5165\u4E00\u4E2A /Game/... \u6216 .uasset \u672C\u5730\u8DEF\u5F84\u3002\n\n")
			TEXT("3. \u70B9\u51FB\u201C\u6DFB\u52A0\u6587\u4EF6\u201D\u6216\u201C\u6DFB\u52A0\u8D44\u6E90\u201D\u53EF\u4EE5\u5C06\u591A\u4E2A\u8D44\u6E90\u8FFD\u52A0\u5230\u5217\u8868\u4E2D\u3002\n\n")
			TEXT("4. \u76EE\u6807\u6839\u8DEF\u5F84\u5FC5\u987B\u5728 /Game/ \u4E0B\uFF0C\u4E14\u4E0D\u80FD\u76F4\u63A5\u5199 /Game \u3002\n\n")
			TEXT("5. \u5F00\u542F\u201C\u6F14\u7EC3\u6A21\u5F0F\uff08\u4E0D\u5B9E\u9645\u590D\u5236/\u5220\u9664\uff09\u201D\u540E\uFF0C\u6267\u884C\u6309\u94AE\u53EA\u4F1A\u751F\u6210\u9884\u89C8\u65E5\u5FD7\uFF0C\u4E0D\u4F1A\u771F\u6B63\u590D\u5236\u6216\u5220\u9664\u3002\n\n")
			TEXT("6. \u5173\u95ED\u201C\u6F14\u7EC3\u6A21\u5F0F\uff08\u4E0D\u5B9E\u9645\u590D\u5236/\u5220\u9664\uff09\u201D\u540E\uFF0C\u63D2\u4EF6\u4F1A\u5728\u6267\u884C\u524D\u663E\u793A\u786E\u8BA4\u5F39\u7A97\u3002\n\n")
			TEXT("7. \u201C\u590D\u5236\u8D44\u6E90\u201D\u53EA\u590D\u5236\u8D44\u6E90\uFF1B\u201C\u590D\u5236\u5E76\u6E05\u7406\u76EE\u6807\u76EE\u5F55\u201D\u4F1A\u989D\u5916\u6E05\u7406\u4E0D\u5C5E\u4E8E\u5F53\u524D\u95ED\u5305\u7684\u76EE\u6807\u8D44\u6E90\u3002"));
	}

	return FText::FromString(
		TEXT("1. Choose the UI language at the top. You can switch between English and Chinese at any time.\n\n")
		TEXT("2. The source input supports multiple assets. Enter one /Game/... path or local .uasset path per line.\n\n")
		TEXT("3. Click Add File or Add Asset to append multiple source assets into the list.\n\n")
		TEXT("4. The target root path must stay under /Game/ and cannot be the full /Game root.\n\n")
		TEXT("5. When Dry Run is enabled, execution buttons only generate preview logs and do not copy or delete assets.\n\n")
		TEXT("6. When Dry Run is disabled, the plugin shows a confirmation dialog before executing copy or cleanup.\n\n")
		TEXT("7. Copy Assets only duplicates assets. Copy + Clean Target also removes target assets that do not belong to the current closure."));
}

FSlateColor SAssetClosureCollectorPanel::GetEnglishButtonColor() const
{
	return CurrentLanguage == EAssetClosureCollectorLanguage::English
		? FAppStyle::Get().GetSlateColor("Colors.AccentBlue")
		: FSlateColor::UseForeground();
}

FSlateColor SAssetClosureCollectorPanel::GetChineseButtonColor() const
{
	return CurrentLanguage == EAssetClosureCollectorLanguage::Chinese
		? FAppStyle::Get().GetSlateColor("Colors.AccentBlue")
		: FSlateColor::UseForeground();
}

ECheckBoxState SAssetClosureCollectorPanel::GetIncludeHardState() const
{
	return AnalyzeOptions.bIncludeHard ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SAssetClosureCollectorPanel::OnIncludeHardChanged(ECheckBoxState NewState)
{
	AnalyzeOptions.bIncludeHard = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SAssetClosureCollectorPanel::GetIncludeSoftState() const
{
	return AnalyzeOptions.bIncludeSoft ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SAssetClosureCollectorPanel::OnIncludeSoftChanged(ECheckBoxState NewState)
{
	AnalyzeOptions.bIncludeSoft = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SAssetClosureCollectorPanel::GetIncludeManageState() const
{
	return AnalyzeOptions.bIncludeManage ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SAssetClosureCollectorPanel::OnIncludeManageChanged(ECheckBoxState NewState)
{
	AnalyzeOptions.bIncludeManage = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SAssetClosureCollectorPanel::GetRecursiveState() const
{
	return AnalyzeOptions.bRecursive ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SAssetClosureCollectorPanel::OnRecursiveChanged(ECheckBoxState NewState)
{
	AnalyzeOptions.bRecursive = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SAssetClosureCollectorPanel::GetDryRunState() const
{
	return bDryRun ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SAssetClosureCollectorPanel::OnDryRunChanged(ECheckBoxState NewState)
{
	bDryRun = (NewState == ECheckBoxState::Checked);
}

FText SAssetClosureCollectorPanel::GetSummaryText() const
{
	if (CurrentLanguage == EAssetClosureCollectorLanguage::Chinese)
	{
		return FText::FromString(FString::Printf(
			TEXT("\u8D44\u6E90\u6570: %d | \u590D\u5236\u8BA1\u5212: %d | \u6E05\u7406\u9879: %d | Dry Run: %s | \u9519\u8BEF: %d | \u8B66\u544A: %d"),
			AnalysisResult.Nodes.Num(),
			CopyPlan.Num(),
			CleanupPlan.Num(),
			bDryRun ? TEXT("\u6F14\u7EC3\u6A21\u5F0F") : TEXT("\u6B63\u5F0F\u6267\u884C"),
			AnalysisResult.Errors.Num() + LastExecutionResult.Errors.Num(),
			AnalysisResult.Warnings.Num()));
	}

	return FText::FromString(FString::Printf(
		TEXT("Assets: %d | Copy Plan: %d | Cleanup: %d | Dry Run: %s | Errors: %d | Warnings: %d"),
		AnalysisResult.Nodes.Num(),
		CopyPlan.Num(),
		CleanupPlan.Num(),
		bDryRun ? TEXT("On") : TEXT("Off"),
		AnalysisResult.Errors.Num() + LastExecutionResult.Errors.Num(),
		AnalysisResult.Warnings.Num()));
}

FText SAssetClosureCollectorPanel::GetResultsText() const
{
	TArray<FString> Lines;
	TArray<FName> SortedKeys;
	AnalysisResult.Nodes.GetKeys(SortedKeys);
	SortedKeys.Sort(FNameLexicalLess());

	if (!SourceInput.IsEmpty())
	{
		Lines.Add(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("\u8F93\u5165\u8D44\u6E90") : TEXT("Input Assets"));
		TArray<FString> SourceLines;
		SourceInput.ParseIntoArrayLines(SourceLines, true);
		for (const FString& SourceLine : SourceLines)
		{
			Lines.Add(SourceLine);
		}
		Lines.Add(TEXT(""));
	}

	for (const FString& Error : AnalysisResult.Errors)
	{
		Lines.Add(FString(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("[\u9519\u8BEF] ") : TEXT("[Error] ")) + Error);
	}

	for (const FString& Warning : AnalysisResult.Warnings)
	{
		Lines.Add(FString(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("[\u8B66\u544A] ") : TEXT("[Warning] ")) + Warning);
	}

	for (const FName& Key : SortedKeys)
	{
		const FAssetClosureNode& Node = AnalysisResult.Nodes[Key];
		if (CurrentLanguage == EAssetClosureCollectorLanguage::Chinese)
		{
			Lines.Add(FString::Printf(
				TEXT("\u8D44\u6E90: %s | \u7C7B: %s | \u5B58\u5728: %s | \u88AB\u5F15\u7528\u6B21\u6570: %d"),
				*Node.PackageName.ToString(),
				*Node.AssetClass,
				Node.bExists ? TEXT("\u662F") : TEXT("\u5426"),
				Node.ReferencedBy.Num()));
		}
		else
		{
			Lines.Add(FString::Printf(
				TEXT("Asset: %s | Class: %s | Exists: %s | ReferencedBy: %d"),
				*Node.PackageName.ToString(),
				*Node.AssetClass,
				Node.bExists ? TEXT("Yes") : TEXT("No"),
				Node.ReferencedBy.Num()));
		}
	}

	if (CopyPlan.Num() > 0)
	{
		Lines.Add(TEXT(""));
		Lines.Add(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("\u590D\u5236\u8BA1\u5212") : TEXT("Copy Plan"));
		for (const FAssetClosureCopyPlanItem& Item : CopyPlan)
		{
			Lines.Add(FString::Printf(TEXT("%s -> %s | %s"), *Item.SourcePackageName.ToString(), *Item.TargetPackageName.ToString(), *Item.Status));
		}
	}

	if (CleanupPlan.Num() > 0)
	{
		Lines.Add(TEXT(""));
		Lines.Add(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("\u76EE\u6807\u76EE\u5F55\u6E05\u7406\u9879") : TEXT("Target Cleanup Plan"));
		for (const FAssetClosureCleanupPlanItem& Item : CleanupPlan)
		{
			Lines.Add(FString::Printf(TEXT("%s | %s"), *Item.TargetPackageName.ToString(), *Item.Status));
		}
	}

	if (LastExecutionResult.Infos.Num() > 0 || LastExecutionResult.Errors.Num() > 0)
	{
		Lines.Add(TEXT(""));
		Lines.Add(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("\u6267\u884C\u7ED3\u679C") : TEXT("Execution Result"));
		for (const FString& Info : LastExecutionResult.Infos)
		{
			Lines.Add(FString(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("[\u5B8C\u6210] ") : TEXT("[Done] ")) + Info);
		}
		for (const FString& Error : LastExecutionResult.Errors)
		{
			Lines.Add(FString(CurrentLanguage == EAssetClosureCollectorLanguage::Chinese ? TEXT("[\u5931\u8D25] ") : TEXT("[Failed] ")) + Error);
		}
	}

	return FText::FromString(FString::Join(Lines, TEXT("\n")));
}
