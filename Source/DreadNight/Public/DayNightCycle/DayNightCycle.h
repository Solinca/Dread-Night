#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.generated.h"

class AMyGameStateBase;
class UExponentialHeightFogComponent;

UCLASS()
class DREADNIGHT_API ADayNightCycle : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle ProcessDayTimer;

	FRotator SunRotation, MoonRotation;

	AMyGameStateBase* MyGameStateBase;

	UExponentialHeightFogComponent* FogComponent = nullptr;

	bool hasDawnEnded, hasDuskStarted = false;

	float CurrentPhaseTimeInSeconds;

	float CurrentPhaseRotation, DawnRotation, DuskRotation, DayRotation;

	void StartDayCycle();

	void ProcessDayPerSecond();

	void StartMoonCycle();
	
protected:
	ADayNightCycle();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UDirectionalLightComponent> Sun = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UDirectionalLightComponent> Moon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class AExponentialHeightFog> Fog = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DayTimeInSeconds = 900;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DawnTimeInSeconds = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DuskTimeInSeconds = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float StartSunRotation = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float FullSunRotation = 360;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float EndSunRotation = -270;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DawnRotationThreshold = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DuskRotationhreshold = -180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float ProcessedTimeInterval = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float MaximalFogDensity = 0.1f;
};
