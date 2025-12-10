#include "UserWidgets/OptionsWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "UserWidgets/MainMenuWidget.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WindowModeMap = {
		{TEXT("Fullscreen"), EWindowMode::Fullscreen},
		{TEXT("Windowed Fullscreen"), EWindowMode::WindowedFullscreen},
		{TEXT("Windowed"), EWindowMode::Windowed}
	};

	TArray<FIntPoint> Array;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Array);
	for (auto& i : Array)
	{
		ResolutionMap.Add(FString::Printf(TEXT("%dx%d"), i.X, i.Y), i);
	}

	GraphicsMap = {
		{TEXT("Low"), 0},
		{TEXT("Medium"), 1},
		{TEXT("High"), 2},
		{TEXT("Epic"), 3},
		{TEXT("Cinematic"), 4},
	};
	
	if (ButtonReturn) ButtonReturn->OnClicked.AddDynamic(this, &UOptionsWidget::OnReturnClicked);

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->LoadSettings();

	if (ComboBoxWindowMode)
	{
		SetupComboBox<EWindowMode::Type>(
			ComboBoxWindowMode,
			WindowModeMap,
			Settings->GetFullscreenMode()
		);

		ComboBoxWindowMode->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnWindowModeChanged);
	}

	if (ComboBoxResolution)
	{
		SetupComboBox<FIntPoint>(
			ComboBoxResolution,
			ResolutionMap,
			Settings->GetScreenResolution()
		);

		ComboBoxResolution->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnResolutionChanged);
	}

	if (ComboBoxGraphics)
	{
		SetupComboBox<int>(
			ComboBoxGraphics,
			GraphicsMap,
			Settings->GetOverallScalabilityLevel()
		);

		ComboBoxGraphics->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnGraphicsChanged);
	}

	if (CheckBoxVSync)
	{
		CheckBoxVSync->SetIsChecked(Settings->bUseVSync);
		CheckBoxVSync->OnCheckStateChanged.AddDynamic(this, &UOptionsWidget::OnCheckboxVSyncChanged);
	}
}

void UOptionsWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!WindowModeMap.Contains(SelectedItem)) return;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	Settings->SetFullscreenMode(WindowModeMap[SelectedItem]);
	Settings->ApplySettings(false);
}

void UOptionsWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!ResolutionMap.Contains(SelectedItem)) return;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	Settings->SetScreenResolution(ResolutionMap[SelectedItem]);
	Settings->ApplySettings(false);
}

void UOptionsWidget::OnGraphicsChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!GraphicsMap.Contains(SelectedItem)) return;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	Settings->SetOverallScalabilityLevel(GraphicsMap[SelectedItem]);
	Settings->ApplySettings(false);
}

void UOptionsWidget::OnCheckboxVSyncChanged(bool bIsChecked)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->bUseVSync = bIsChecked;
	Settings->ApplySettings(false);
}

void UOptionsWidget::OnReturnClicked()
{
	OnReturn.Broadcast();

	if (!MainMenuWidgetClass) return;

	if (UMainMenuWidget* MMW = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass))
	{
		MMW->AddToViewport();
		MainMenuWidgetClass = nullptr;
		RemoveFromParent();
	}
}

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UOptionsWidget::SetMainMenuWidgetClass(TSubclassOf<UMainMenuWidget> MMWC)
{
	MainMenuWidgetClass = MMWC;
}
