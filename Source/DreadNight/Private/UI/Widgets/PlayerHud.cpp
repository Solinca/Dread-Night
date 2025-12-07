#include "UI/Widgets/PlayerHud.h"
#include "UI/Widgets/RadialProgressBarImage.h"
#include "UI/Widgets/SmartProgressBar.h"

void UPlayerHud::UpdateHealthBar(const float CurrentValue, const float MaxValue)
{
	HealthBar->UpdateValue(CurrentValue, MaxValue);
}

void UPlayerHud::UpdateStaminaBar(const float CurrentValue, const float MaxValue)
{
	StaminaBar->UpdateValue(CurrentValue, MaxValue);
}

void UPlayerHud::UpdateThirstRadialBarImage(const float CurrentValue)
{
	if (ThirstRadialBarImage)
	{
		ThirstRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateHungerRadialBarImage(const float CurrentValue)
{
	if (HungerRadialBarImage)
	{
		HungerRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateMentalRadialBarImage(const float CurrentValue)
{
	if (MentalRadialBarImage)
	{
		MentalRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateFearRadialBarImage(const float CurrentValue)
{
	if (FearRadialBarImage)
	{
		FearRadialBarImage->UpdateValue(CurrentValue);
	}
}
