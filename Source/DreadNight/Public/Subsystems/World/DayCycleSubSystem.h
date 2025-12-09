#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DayCycleSubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNightStartSignature);

class ABaseLevelWorldSettings;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;

UCLASS()
class DREADNIGHT_API UDayCycleSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	FTimerHandle ProcessDayTimer;

	FRotator SunRotation, MoonRotation;

	ABaseLevelWorldSettings* BaseWorldSettings = nullptr;

	UDirectionalLightComponent* Sun = nullptr;

	UDirectionalLightComponent* Moon = nullptr;

	UExponentialHeightFogComponent* FogComponent = nullptr;

	USkyAtmosphereComponent* Sky = nullptr;

	UVolumetricCloudComponent* Cloud = nullptr;

	bool hasDawnEnded, hasDuskStarted = false;

	float CurrentPhaseTimeInSeconds;

	float CurrentPhaseRotation, DawnRotation, DuskRotation, DayRotation;

	UFUNCTION()
	void StartDayCycle();

	void ProcessDayPerSecond();

	void StartMoonCycle();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	FOnNightStartSignature OnNightStart;
};
