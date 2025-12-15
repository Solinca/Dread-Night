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
class ULightComponent;
class UNiagaraComponent;

UCLASS()
class DREADNIGHT_API UDayCycleSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	FTimerHandle ProcessDayTimer;

	FTimerHandle WidgetSpawnDelayTimerHandle;

	FRotator SunRotation, MoonRotation;

	ABaseLevelWorldSettings* BaseWorldSettings = nullptr;

	UDirectionalLightComponent* Sun = nullptr;

	UDirectionalLightComponent* Moon = nullptr;

	UExponentialHeightFogComponent* FogComponent = nullptr;

	USkyAtmosphereComponent* Sky = nullptr;

	UVolumetricCloudComponent* Cloud = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> CurrentWidget;
	
	bool hasDawnEnded, hasDuskStarted = false;

	float CurrentPhaseTimeInSeconds;

	float CurrentPhaseRotation, DawnRotation, DuskRotation, DayRotation;

	int DayCounter = 0;

	TMap<ULightComponent*, float> LightList;

	TArray<UNiagaraComponent*> VFXList;

	UFUNCTION()
	void StartDayCycle();

	void ProcessDayPerSecond();

	void StartMoonCycle();

	void InitSunDirectionalLight(UWorld& InWorld);

	void InitMoonDirectionalLight(UWorld& InWorld);

	void InitExponentialHeightFog(UWorld& InWorld);

	void InitVolumetricCloud(UWorld& InWorld);

	void InitSkyAtmoshpere(UWorld& InWorld);
	
	void SpawnPopUpWidget(const FString& InKey);

	void ProcessEveryLightSource();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	FOnNightStartSignature OnNightStart;

	static ThisClass* Get(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "DayCycle")
	int GetDayCounter() const { return DayCounter; }

	void RegisterLightSource(ULightComponent* Light, UNiagaraComponent* VFX);
};
