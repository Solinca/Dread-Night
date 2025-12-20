#include "UserWidgets/OptionsWidget.h"
#include "Global/MyGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
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

	MySettings = Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings());

	if (ComboBoxWindowMode)
	{
		SetupComboBox<EWindowMode::Type>(
			ComboBoxWindowMode,
			WindowModeMap,
			MySettings->GetFullscreenMode()
		);

		ComboBoxWindowMode->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnWindowModeChanged);
	}

	if (ComboBoxResolution)
	{
		SetupComboBox<FIntPoint>(
			ComboBoxResolution,
			ResolutionMap,
			MySettings->GetScreenResolution()
		);

		ComboBoxResolution->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnResolutionChanged);
	}

	if (ComboBoxGraphics)
	{
		SetupComboBox<int>(
			ComboBoxGraphics,
			GraphicsMap,
			MySettings->GetOverallScalabilityLevel()
		);

		ComboBoxGraphics->OnSelectionChanged.AddDynamic(this, &UOptionsWidget::OnGraphicsChanged);
	}

	if (CheckBoxVSync)
	{
		CheckBoxVSync->SetIsChecked(MySettings->bUseVSync);
		
		CheckBoxVSync->OnCheckStateChanged.AddDynamic(this, &UOptionsWidget::OnCheckboxVSyncChanged);
	}

	if (CameraSensitivity)
	{
		CameraSensitivity->SetValue(MySettings->GetPlayerCameraSensitivity());

		CameraSensitivity->OnValueChanged.AddDynamic(this, &UOptionsWidget::OnCameraSensitivityChanged);

		CameraSensitivity->OnMouseCaptureEnd.AddDynamic(this, &UOptionsWidget::OnCameraSensitivityCaptureEnd);

		if (CameraSensitivityValue)
		{
			CameraSensitivityValue->SetText(FText::FromString(FString::FromInt(MySettings->GetPlayerCameraSensitivity())));
		}
	}

	if (MusicVolume)
	{
		MusicVolume->SetValue(MySettings->GetMusicVolume());

		MusicVolume->OnValueChanged.AddDynamic(this, &UOptionsWidget::OnMusicVolumeChanged);

		MusicVolume->OnMouseCaptureEnd.AddDynamic(this, &UOptionsWidget::OnMusicVolumeCaptureEnd);

		if (MusicVolumeValue)
		{
			MusicVolumeValue->SetText(FText::FromString(FString::FromInt(MySettings->GetMusicVolume())));
		}
	}

	if (SFXVolume)
	{
		SFXVolume->SetValue(MySettings->GetSFXVolume());

		SFXVolume->OnValueChanged.AddDynamic(this, &UOptionsWidget::OnSFXVolumeChanged);

		SFXVolume->OnMouseCaptureEnd.AddDynamic(this, &UOptionsWidget::OnSFXVolumeCaptureEnd);

		if (SFXVolumeValue)
		{
			SFXVolumeValue->SetText(FText::FromString(FString::FromInt(MySettings->GetSFXVolume())));
		}
	}
}

void UOptionsWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!WindowModeMap.Contains(SelectedItem)) return;

	MySettings->SetFullscreenMode(WindowModeMap[SelectedItem]);

	if (MySettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
	{
		const FString NewResolution = ComboBoxResolution->GetOptionAtIndex(ResolutionMap.Num() - 1);
		
		ComboBoxResolution->SetSelectedIndex(ResolutionMap.Num() - 1);
		
		MySettings->SetScreenResolution(ResolutionMap[NewResolution]);
	}
	
	MySettings->ApplySettings(false);
	
	MySettings->SaveSettings();
}

void UOptionsWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!ResolutionMap.Contains(SelectedItem))
	{
		return;
	}

	MySettings->SetScreenResolution(ResolutionMap[SelectedItem]);
	
	MySettings->ApplySettings(false);
	
	MySettings->SaveSettings();
}

void UOptionsWidget::OnGraphicsChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!GraphicsMap.Contains(SelectedItem))
	{
		return;
	}

	MySettings->SetOverallScalabilityLevel(GraphicsMap[SelectedItem]);
	
	MySettings->ApplySettings(false);
	
	MySettings->SaveSettings();
}

void UOptionsWidget::OnCheckboxVSyncChanged(bool bIsChecked)
{
	MySettings->bUseVSync = bIsChecked;
	
	MySettings->ApplySettings(false);
	
	MySettings->SaveSettings();
}

void UOptionsWidget::OnCameraSensitivityChanged(float value)
{
	CameraSensitivityValue->SetText(FText::FromString(FString::FromInt(value)));

	MySettings->SetPlayerCameraSensitivity(value);
}

void UOptionsWidget::OnCameraSensitivityCaptureEnd()
{
	MySettings->SaveSettings();
}

void UOptionsWidget::OnMusicVolumeChanged(float value)
{
	MusicVolumeValue->SetText(FText::FromString(FString::FromInt(value)));

	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MusicSoundMix, MusicSoundClass, MySettings->GetMusicVolume() / 100, 1, 0);

	MySettings->SetMusicVolume(value);
}

void UOptionsWidget::OnMusicVolumeCaptureEnd()
{
	MySettings->SaveSettings();
}

void UOptionsWidget::OnSFXVolumeChanged(float value)
{
	SFXVolumeValue->SetText(FText::FromString(FString::FromInt(value)));

	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SFXSoundMix, SFXSoundClass, MySettings->GetSFXVolume() / 100, 1, 0);

	MySettings->SetSFXVolume(value);
}

void UOptionsWidget::OnSFXVolumeCaptureEnd()
{
	MySettings->SaveSettings();
}

void UOptionsWidget::OnReturnClicked()
{
	OnReturn.Broadcast();

	if (!MainMenuWidgetClass)
	{
		return;
	}

	if (UMainMenuWidget* MMW = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass))
	{
		MMW->AddToViewport();
		
		MainMenuWidgetClass = nullptr;
		
		RemoveFromParent();
	}
}

void UOptionsWidget::SetMainMenuWidgetClass(TSubclassOf<UMainMenuWidget> MMWC)
{
	MainMenuWidgetClass = MMWC;
}
