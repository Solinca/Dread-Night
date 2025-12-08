#include "UserWidgets/OptionsWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "UserWidgets/MainMenuWidget.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonReturn->OnClicked.AddDynamic(this, &UOptionsWidget::OnReturnClicked);

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->LoadSettings();

	WindowModeMap.Add("Fullscreen", EWindowMode::Fullscreen);
	WindowModeMap.Add("Windowed Fullscreen", EWindowMode::WindowedFullscreen);
	WindowModeMap.Add("Windowed", EWindowMode::Windowed);

	if (ComboBoxWindowMode)
	{
		ComboBoxWindowMode->ClearOptions();

		for (auto& Pair : WindowModeMap) ComboBoxWindowMode->AddOption(Pair.Key);

		EWindowMode::Type DefaultWindowMode = Settings->GetFullscreenMode();
		FString DefaultOption;

		switch (DefaultWindowMode)
		{
			case EWindowMode::Fullscreen: DefaultOption = "Fullscreen"; break;
			case EWindowMode::WindowedFullscreen: DefaultOption = "Windowed Fullscreen"; break;
			case EWindowMode::Windowed: DefaultOption = "Windowed"; break;
		}

		ComboBoxWindowMode->SetSelectedOption(DefaultOption);
		ComboBoxWindowMode->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnWindowModeChanged);
	}

	ResolutionMap.Add("1200x720", FIntPoint(1200, 720));
	ResolutionMap.Add("1600x900", FIntPoint(1600, 900));
	ResolutionMap.Add("1920x1080", FIntPoint(1920, 1080));
	ResolutionMap.Add("2560x1440", FIntPoint(2560, 1440));
	ResolutionMap.Add("3840x2160", FIntPoint(3840, 2160));

	if (ComboBoxResolution)
	{
		ComboBoxResolution->ClearOptions();

		for (auto& Pair : ResolutionMap) ComboBoxResolution->AddOption(Pair.Key);

		FIntPoint ScreenResolution = Settings->GetScreenResolution();
		FString DefaultOption;

		for (const TPair<FString, FIntPoint>& Pair : ResolutionMap)
		{
			if (Pair.Value == ScreenResolution)
			{
				DefaultOption = Pair.Key;
				break;
			}
		}

		if (!DefaultOption.IsEmpty())
		{
			ComboBoxResolution->SetSelectedOption(DefaultOption);
			ComboBoxResolution->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnResolutionChanged);
		}
	}

	GraphicsMap.Add("Low", 0);
	GraphicsMap.Add("Medium", 1);
	GraphicsMap.Add("High", 2);
	GraphicsMap.Add("Epic", 3);
	GraphicsMap.Add("Cinematic", 4);

	if (ComboBoxGraphics)
	{
		ComboBoxGraphics->ClearOptions();

		for (auto& Pair : GraphicsMap) ComboBoxGraphics->AddOption(Pair.Key);

		int GraphicIndex = Settings->GetOverallScalabilityLevel();
		FString GraphicsOption;

		for (const TPair<FString, FIntPoint>& Pair : GraphicsMap)
		{
			if (Pair.Value == GraphicIndex)
			{
				GraphicsOption = Pair.Key;
				break;
			}
		}

		if (!GraphicsOption.IsEmpty())
		{
			ComboBoxGraphics->SetSelectedOption(GraphicsOption);
			ComboBoxGraphics->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnGraphicsChanged);
		}
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
	RemoveFromParent();
	UMainMenuWidget* MMW = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	if (MMW) MMW->AddToViewport();
}