#include "Subsystems/World/DayCycleSubSystem.h"
#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/LightComponent.h"
#include "Data/DayCycleSystem/DayCycleSystemDataAsset.h"
#include "Blueprint/UserWidget.h"

void UDayCycleSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BaseWorldSettings = Cast<ABaseLevelWorldSettings>(InWorld.GetWorldSettings());

	DawnRotation = FMath::Abs(BaseWorldSettings->DayCycleSystemData->StartSunRotation - BaseWorldSettings->DayCycleSystemData->DawnRotationThreshold);

	DuskRotation = FMath::Abs(BaseWorldSettings->DayCycleSystemData->EndSunRotation - BaseWorldSettings->DayCycleSystemData->DuskRotationhreshold);

	DayRotation = BaseWorldSettings->DayCycleSystemData->FullSunRotation - DawnRotation - DuskRotation;

	InitSunDirectionalLight(InWorld);

	InitMoonDirectionalLight(InWorld);

	InitExponentialHeightFog(InWorld);

	InitVolumetricCloud(InWorld);

	InitSkyAtmoshpere(InWorld);

	UWaveWorldSubsystem::Get(&InWorld)->OnWaveEnd.AddDynamic(this, &UDayCycleSubSystem::StartDayCycle);

	StartDayCycle();
}

bool UDayCycleSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->IsGameWorld() && World == Cast<ABaseLevelWorldSettings>(World->GetWorldSettings())->MapToLoadSubsystems;
}

void UDayCycleSubSystem::StartDayCycle()
{
	DayCounter++;
	
	SpawnPopUpWidget("WBP_NewDay");
	
	hasDawnEnded = hasDuskStarted = false;

	CurrentPhaseTimeInSeconds = BaseWorldSettings->DayCycleSystemData->DawnTimeInSeconds;

	CurrentPhaseRotation = DawnRotation;

	SunRotation = FRotator(BaseWorldSettings->DayCycleSystemData->StartSunRotation, 0, 0);

	Sun->SetWorldRotation(SunRotation);

	MoonRotation = FRotator(BaseWorldSettings->DayCycleSystemData->StartSunRotation - DayRotation, 0, 0);

	Moon->SetWorldRotation(MoonRotation);

	FogComponent->SetFogDensity(BaseWorldSettings->DayCycleSystemData->MaximalFogDensity);

	GetWorld()->GetTimerManager().SetTimer(ProcessDayTimer, this, &UDayCycleSubSystem::ProcessDayPerSecond, BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval, true);
}

void UDayCycleSubSystem::ProcessDayPerSecond()
{
	FRotator FrameRotation = FRotator((CurrentPhaseRotation / CurrentPhaseTimeInSeconds) * BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval, 0, 0);

	SunRotation -= FrameRotation;

	Sun->SetWorldRotation(SunRotation);

	MoonRotation -= FrameRotation;

	Moon->SetWorldRotation(MoonRotation);

	ProcessEveryLightSource();

	if (hasDuskStarted)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity + (BaseWorldSettings->DayCycleSystemData->MaximalFogDensity / CurrentPhaseTimeInSeconds) * BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval);
	}

	if (!hasDawnEnded)
	{
		FogComponent->SetFogDensity(FogComponent->FogDensity - (BaseWorldSettings->DayCycleSystemData->MaximalFogDensity / CurrentPhaseTimeInSeconds) * BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval);
	}

	if (!hasDuskStarted && SunRotation.Pitch < BaseWorldSettings->DayCycleSystemData->DuskRotationhreshold)
	{
		CurrentPhaseTimeInSeconds = BaseWorldSettings->DayCycleSystemData->DuskTimeInSeconds;

		CurrentPhaseRotation = DuskRotation;

		hasDuskStarted = true;
	}
	else if (!hasDawnEnded && SunRotation.Pitch < BaseWorldSettings->DayCycleSystemData->DawnRotationThreshold)
	{
		CurrentPhaseTimeInSeconds = BaseWorldSettings->DayCycleSystemData->DayTimeInSeconds;

		CurrentPhaseRotation = DayRotation;

		hasDawnEnded = true;
	}

	if (SunRotation.Pitch < BaseWorldSettings->DayCycleSystemData->EndSunRotation)
	{
		StartMoonCycle();
	}
}

void UDayCycleSubSystem::StartMoonCycle()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ProcessDayTimer);

	OnNightStart.Broadcast();

	TimerManager.SetTimer(WidgetSpawnDelayTimerHandle, [this]
	{
		SpawnPopUpWidget("WBP_NewWave");
	}, BaseWorldSettings->DayCycleSystemData->WidgetSpawnDelay, false);
}

void UDayCycleSubSystem::InitSunDirectionalLight(UWorld& InWorld)
{
	Sun = InWorld.SpawnActor<ADirectionalLight>()->GetComponentByClass<UDirectionalLightComponent>();

	Sun->SetMobility(EComponentMobility::Movable);

	Sun->SetIntensity(BaseWorldSettings->DayCycleSystemData->SunIntensity);

	Sun->SetLightColor(BaseWorldSettings->DayCycleSystemData->SunColor);

	Sun->SetCastShadows(BaseWorldSettings->DayCycleSystemData->DoesSunCastShadow);
}

void UDayCycleSubSystem::InitMoonDirectionalLight(UWorld& InWorld)
{
	Moon = InWorld.SpawnActor<ADirectionalLight>()->GetComponentByClass<UDirectionalLightComponent>();

	Moon->SetMobility(EComponentMobility::Movable);

	Moon->SetIntensity(BaseWorldSettings->DayCycleSystemData->MoonIntensity);

	Moon->SetLightColor(BaseWorldSettings->DayCycleSystemData->MoonColor);

	Moon->SetCastShadows(BaseWorldSettings->DayCycleSystemData->DoesMoonCastShadow);

	Moon->SetForwardShadingPriority(1);

	Moon->SetAtmosphereSunLightIndex(1);
}

void UDayCycleSubSystem::InitExponentialHeightFog(UWorld& InWorld)
{
	FogComponent = InWorld.SpawnActor<AExponentialHeightFog>()->GetComponentByClass<UExponentialHeightFogComponent>();

	FogComponent->SetFogDensity(BaseWorldSettings->DayCycleSystemData->MaximalFogDensity);

	FogComponent->SetFogHeightFalloff(BaseWorldSettings->DayCycleSystemData->FogHeightFalloff);

	FogComponent->SetSecondFogDensity(BaseWorldSettings->DayCycleSystemData->MaximalFogDensity);

	if (BaseWorldSettings->DayCycleSystemData->HasVolumetricFog)
	{
		FogComponent->SetVolumetricFog(BaseWorldSettings->DayCycleSystemData->HasVolumetricFog);

		FogComponent->SetVolumetricFogAlbedo(BaseWorldSettings->DayCycleSystemData->VolumetricFogAlbedo.ToFColor(false));

		FogComponent->SetVolumetricFogExtinctionScale(BaseWorldSettings->DayCycleSystemData->VolumetricFogExtinctionScale);
	}
}

void UDayCycleSubSystem::InitVolumetricCloud(UWorld& InWorld)
{
	Cloud = InWorld.SpawnActor<AVolumetricCloud>()->GetComponentByClass<UVolumetricCloudComponent>();
}

void UDayCycleSubSystem::InitSkyAtmoshpere(UWorld& InWorld)
{
	Sky = InWorld.SpawnActor<ASkyAtmosphere>()->GetComponentByClass<USkyAtmosphereComponent>();

	Sky->SetRayleighScatteringScale(BaseWorldSettings->DayCycleSystemData->RayleighScatteringScale);
}

void UDayCycleSubSystem::SpawnPopUpWidget(const FString& InKey)
{
	if (!BaseWorldSettings->DayCycleSystemData->WidgetToSpawn.Contains(InKey))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Key '%s' is missing from BaseWorldSettings configuration."), *FString(__FUNCTION__), *InKey);
	}
	
	CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(this, 0), BaseWorldSettings->DayCycleSystemData->WidgetToSpawn[InKey]);

	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}
}

UDayCycleSubSystem::ThisClass* UDayCycleSubSystem::Get(UObject* WorldContext)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetSubsystem<UDayCycleSubSystem>();
	}

	return nullptr;
}


void UDayCycleSubSystem::RegisterLightSource(ULightComponent* Light)
{
	LightList.Add(Light, Light->Intensity);
}

void UDayCycleSubSystem::ProcessEveryLightSource()
{
	if (hasDuskStarted)
	{
		for (TPair<ULightComponent*, float> Pair : LightList)
		{
			Pair.Key->SetIntensity(Pair.Key->Intensity + (Pair.Value / CurrentPhaseTimeInSeconds) * BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval);
		}
	}

	if (!hasDawnEnded)
	{
		for (TPair<ULightComponent*, float> Pair : LightList)
		{
			Pair.Key->SetIntensity(Pair.Key->Intensity - (Pair.Value / CurrentPhaseTimeInSeconds) * BaseWorldSettings->DayCycleSystemData->ProcessedTimeInterval);
		}
	}
}
