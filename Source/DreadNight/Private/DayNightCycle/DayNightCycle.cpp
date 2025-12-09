#include "DayNightCycle/DayNightCycle.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Kismet/GameplayStatics.h"
#include "Global/MyGameStateBase.h"

ADayNightCycle::ADayNightCycle()
{
	PrimaryActorTick.bCanEverTick = false;

	Moon = CreateDefaultSubobject<UDirectionalLightComponent>("Moon");

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>("Sun");
}

void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();

	FogComponent = Fog->GetComponentByClass<UExponentialHeightFogComponent>();

	FogComponent->SetFogDensity(MaximalFogDensity);

	DawnRotation = FMath::Abs(StartSunRotation - DawnRotationThreshold);

	DuskRotation = FMath::Abs(EndSunRotation - DuskRotationhreshold);

	DayRotation = FullSunRotation - DawnRotation - DuskRotation;

	MyGameStateBase = Cast<AMyGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	MyGameStateBase->OnWaveEnd.AddDynamic(this, &ADayNightCycle::StartDayCycle);

	StartDayCycle();
}

void ADayNightCycle::StartDayCycle()
{
	hasDawnEnded = hasDuskStarted = false;

	CurrentPhaseTimeInSeconds = DawnTimeInSeconds;

	CurrentPhaseRotation = DawnRotation;

	SunRotation = FRotator(StartSunRotation, 0, 0);

	Sun->SetWorldRotation(SunRotation);

	MoonRotation = FRotator(StartSunRotation - DayRotation, 0, 0);

	Moon->SetWorldRotation(MoonRotation);

	FogComponent->SetFogDensity(MaximalFogDensity);

	GetWorldTimerManager().SetTimer(ProcessDayTimer, this, &ADayNightCycle::ProcessDayPerSecond, ProcessedTimeInterval, true);
}

void ADayNightCycle::ProcessDayPerSecond()
{
	FRotator FrameRotation = FRotator((CurrentPhaseRotation / CurrentPhaseTimeInSeconds) * ProcessedTimeInterval, 0, 0);

	SunRotation -= FrameRotation;

	MoonRotation -= FrameRotation;

	Sun->SetWorldRotation(SunRotation);

	Moon->SetWorldRotation(MoonRotation);

	if (hasDuskStarted)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity + (MaximalFogDensity / CurrentPhaseTimeInSeconds) * ProcessedTimeInterval);
	}

	if (!hasDawnEnded)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity - (MaximalFogDensity / CurrentPhaseTimeInSeconds) * ProcessedTimeInterval);
	}

	if (!hasDuskStarted && SunRotation.Pitch < DuskRotationhreshold)
	{
		CurrentPhaseTimeInSeconds = DuskTimeInSeconds;

		CurrentPhaseRotation = DuskRotation;

		hasDuskStarted = true;
	}
	else if (!hasDawnEnded && SunRotation.Pitch < DawnRotationThreshold)
	{
		CurrentPhaseTimeInSeconds = DayTimeInSeconds;

		CurrentPhaseRotation = DayRotation;

		hasDawnEnded = true;
	}

	if (SunRotation.Pitch < EndSunRotation)
	{
		StartMoonCycle();
	}
}

void ADayNightCycle::StartMoonCycle()
{
	GetWorldTimerManager().ClearTimer(ProcessDayTimer);

	MyGameStateBase->OnNightStart.Broadcast();
}
