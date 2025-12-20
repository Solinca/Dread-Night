#include "UI/Widgets/DayNightSlider.h"

#include "Components/Slider.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/World/DayCycleSubSystem.h"


void UDayNightSlider::NativeConstruct()
{
	Super::NativeConstruct();

	if (const ABaseLevelWorldSettings* BaseLevelWorldSettings = Cast<ABaseLevelWorldSettings>(GetWorld()->GetWorldSettings()))
	{
		DayNightSlider->SetMinValue(0.f);
		DayNightSlider->SetMaxValue(1.f);
		FullDayTime = BaseLevelWorldSettings->DayCycleSystemData->DayTimeInSeconds +
			BaseLevelWorldSettings->DayCycleSystemData->DawnTimeInSeconds +
			BaseLevelWorldSettings->DayCycleSystemData->DuskTimeInSeconds;
	}

	if (UDayCycleSubSystem* DayCycleSubSystem = GetWorld()->GetSubsystem<UDayCycleSubSystem>())
	{
		DayCycleSubSystem->OnDawnStart.AddDynamic(this, &ThisClass::StartDayCycle);
		DayCycleSubSystem->OnNightStart.AddDynamic(this, &ThisClass::NightStart);
	}
}

void UDayNightSlider::StartDayCycle()
{
	bIsDayCycle = true;
	CurrentDayTime = 0.f;
	DayNightSlider->SetValue(CurrentDayTime);
}

void UDayNightSlider::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bIsDayCycle || UGameplayStatics::IsGamePaused(this))
	{
		return;
	}
	
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentDayTime += InDeltaTime;

	const float Time = CurrentDayTime / FullDayTime;
	DayNightSlider->SetValue(Time);
}
