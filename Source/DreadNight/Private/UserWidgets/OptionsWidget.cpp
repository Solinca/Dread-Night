#include "UserWidgets/OptionsWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "UserWidgets/MainMenuWidget.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

template<typename TValue>
void UOptionsWidget::SetupComboBox(
	UComboBoxString* ComboBox,
	TMap<FString, TValue>& Map,
	const TValue& CurrentValue)
{
	if (!ComboBox) return;

	ComboBox->ClearOptions();

	for (const auto& Pair : Map) ComboBox->AddOption(Pair.Key);

	for (const auto& Pair : Map)
	{
		if (Pair.Value == CurrentValue)
		{
			ComboBox->SetSelectedOption(Pair.Key);
			break;
		}
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
	RemoveFromParent();

	if (UMainMenuWidget* MMW = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass))
	{
		MMW->AddToViewport();
	}
}

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WindowModeMap = {
		{TEXT("Fullscreen"), EWindowMode::Fullscreen},
		{TEXT("Windowed Fullscreen"), EWindowMode::WindowedFullscreen},
		{TEXT("Windowed"), EWindowMode::Windowed}
	};

	ResolutionMap = {
		{TEXT("1200x720"), FIntPoint(1200, 720)},
		{TEXT("1600x900"), FIntPoint(1600, 900)},
		{TEXT("1920x1080"), FIntPoint(1920, 1080)},
		{TEXT("2560x1440"), FIntPoint(2560, 1440)},
		{TEXT("3840x2160"), FIntPoint(3840, 2160)}
	};

	GraphicsMap = {
		{TEXT("Low"), 0},
		{TEXT("Medium"), 1},
		{TEXT("High"), 2},
		{TEXT("Epic"), 3},
		{TEXT("Cinematic"), 4},
	};
}