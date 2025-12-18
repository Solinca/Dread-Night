#include "Global/MyGameUserSettings.h"

UMyGameUserSettings::UMyGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraSensitivity = 75.f;
}

void UMyGameUserSettings::SetPlayerCameraSensitivity(float value)
{
	PlayerCameraSensitivity = value;
}
