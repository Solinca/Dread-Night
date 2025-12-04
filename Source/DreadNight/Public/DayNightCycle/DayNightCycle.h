#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "DayNightCycle.generated.h"

UCLASS()
class DREADNIGHT_API ADayNightCycle : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle DayTimer;

	FRotator SunRotation, MoonRotation;

	UExponentialHeightFogComponent* FogComponent = nullptr;

	bool hasDawnEnded, hasDuskStarted = false;

	float CurrentPhaseTimeInSeconds;

	float CurrentPhaseRotation, DawnRotation, DuskRotation, DayRotation;

	UFUNCTION()
	void StartDayCycle();

	void ProcessDayPerSecond();

	UFUNCTION()
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
	float DayTimeInSeconds = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DawnTimeInSeconds = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DuskTimeInSeconds = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float StartSunRotation = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float FullSunRotation = 360;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float EndSunRotation = -270;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DawnRotationThreshold = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float DuskRotationhreshold = -190;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float ProcessedTimeInterval = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float MaximalFogDensity = 0.1f;
};
