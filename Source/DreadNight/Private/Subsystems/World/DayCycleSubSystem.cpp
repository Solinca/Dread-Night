#include "Subsystems/World/DayCycleSubSystem.h"
#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"

void UDayCycleSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BaseWorldSettings = Cast<ABaseLevelWorldSettings>(InWorld.GetWorldSettings());

    Sun = InWorld.SpawnActor<ADirectionalLight>()->GetComponentByClass<UDirectionalLightComponent>();
	
	Sun->SetMobility(EComponentMobility::Movable);
	Sun->SetIntensity(BaseWorldSettings->SunIntensity);
	Sun->SetLightColor(BaseWorldSettings->SunColor);
	Sun->SetCastShadows(BaseWorldSettings->DoesSunCastShadow);

	Moon = InWorld.SpawnActor<ADirectionalLight>()->GetComponentByClass<UDirectionalLightComponent>();

	Moon->SetMobility(EComponentMobility::Movable);
	Moon->SetIntensity(BaseWorldSettings->MoonIntensity);
	Moon->SetLightColor(BaseWorldSettings->MoonColor);
	Moon->SetCastShadows(BaseWorldSettings->DoesMoonCastShadow);
	Moon->SetForwardShadingPriority(1);
	Moon->SetAtmosphereSunLightIndex(1);

	FogComponent = InWorld.SpawnActor<AExponentialHeightFog>()->GetComponentByClass<UExponentialHeightFogComponent>();

	FogComponent->SetFogDensity(BaseWorldSettings->MaximalFogDensity);
	FogComponent->SetFogHeightFalloff(BaseWorldSettings->FogHeightFalloff);
	FogComponent->SetSecondFogDensity(BaseWorldSettings->MaximalFogDensity);

	if (BaseWorldSettings->HasVolumetricFog)
	{
		FogComponent->SetVolumetricFog(BaseWorldSettings->HasVolumetricFog);
		FogComponent->SetVolumetricFogAlbedo(BaseWorldSettings->VolumetricFogAlbedo.ToFColor(false));
		FogComponent->SetVolumetricFogExtinctionScale(BaseWorldSettings->VolumetricFogExtinctionScale);
	}

	Cloud = InWorld.SpawnActor<AVolumetricCloud>()->GetComponentByClass<UVolumetricCloudComponent>();

	Sky = InWorld.SpawnActor<ASkyAtmosphere>()->GetComponentByClass<USkyAtmosphereComponent>();

	Sky->SetRayleighScatteringScale(0.1f);

	DawnRotation = FMath::Abs(BaseWorldSettings->StartSunRotation - BaseWorldSettings->DawnRotationThreshold);

	DuskRotation = FMath::Abs(BaseWorldSettings->EndSunRotation - BaseWorldSettings->DuskRotationhreshold);

	DayRotation = BaseWorldSettings->FullSunRotation - DawnRotation - DuskRotation;

	InWorld.GetSubsystem<UWaveWorldSubsystem>()->OnWaveEnd.AddDynamic(this, &UDayCycleSubSystem::StartDayCycle);

	StartDayCycle();
}

bool UDayCycleSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->IsGameWorld() && World->GetMapName().Contains(Cast<ABaseLevelWorldSettings>(World->GetWorldSettings())->MapName);
}

void UDayCycleSubSystem::StartDayCycle()
{
	hasDawnEnded = hasDuskStarted = false;

	CurrentPhaseTimeInSeconds = BaseWorldSettings->DawnTimeInSeconds;

	CurrentPhaseRotation = DawnRotation;

	SunRotation = FRotator(BaseWorldSettings->StartSunRotation, 0, 0);

	Sun->SetWorldRotation(SunRotation);

	MoonRotation = FRotator(BaseWorldSettings->StartSunRotation - DayRotation, 0, 0);

	Moon->SetWorldRotation(MoonRotation);

	FogComponent->SetFogDensity(BaseWorldSettings->MaximalFogDensity);

	GetWorld()->GetTimerManager().SetTimer(ProcessDayTimer, this, &UDayCycleSubSystem::ProcessDayPerSecond, BaseWorldSettings->ProcessedTimeInterval, true);
}

void UDayCycleSubSystem::ProcessDayPerSecond()
{
	FRotator FrameRotation = FRotator((CurrentPhaseRotation / CurrentPhaseTimeInSeconds) * BaseWorldSettings->ProcessedTimeInterval, 0, 0);

	SunRotation -= FrameRotation;

	Sun->SetWorldRotation(SunRotation);

	MoonRotation -= FrameRotation;

	Moon->SetWorldRotation(MoonRotation);

	if (hasDuskStarted)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity + (BaseWorldSettings->MaximalFogDensity / CurrentPhaseTimeInSeconds) * BaseWorldSettings->ProcessedTimeInterval);
	}

	if (!hasDawnEnded)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity - (BaseWorldSettings->MaximalFogDensity / CurrentPhaseTimeInSeconds) * BaseWorldSettings->ProcessedTimeInterval);
	}

	if (!hasDuskStarted && SunRotation.Pitch < BaseWorldSettings->DuskRotationhreshold)
	{
		CurrentPhaseTimeInSeconds = BaseWorldSettings->DuskTimeInSeconds;

		CurrentPhaseRotation = DuskRotation;

		hasDuskStarted = true;
	}
	else if (!hasDawnEnded && SunRotation.Pitch < BaseWorldSettings->DawnRotationThreshold)
	{
		CurrentPhaseTimeInSeconds = BaseWorldSettings->DayTimeInSeconds;

		CurrentPhaseRotation = DayRotation;

		hasDawnEnded = true;
	}

	if (SunRotation.Pitch < BaseWorldSettings->EndSunRotation)
	{
		StartMoonCycle();
	}
}

void UDayCycleSubSystem::StartMoonCycle()
{
	GetWorld()->GetTimerManager().ClearTimer(ProcessDayTimer);

	OnNightStart.Broadcast();
}
