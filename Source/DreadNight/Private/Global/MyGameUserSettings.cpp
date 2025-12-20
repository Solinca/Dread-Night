#include "Global/MyGameUserSettings.h"

UMyGameUserSettings::UMyGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraSensitivity = 75.f;

	MusicVolume = 100.f;

	SFXVolume = 100.f;
}

void UMyGameUserSettings::SetPlayerCameraSensitivity(float value)
{
	PlayerCameraSensitivity = value;
}

void UMyGameUserSettings::SetMusicVolume(float value)
{
	MusicVolume = value;
}

void UMyGameUserSettings::SetSFXVolume(float value)
{
	SFXVolume = value;
}
