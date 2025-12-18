#pragma once

#include "CoreMinimal.h"
#include "SaveSystem/SavableObject.h"
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
class DREADNIGHT_API UDayCycleSubSystem : public UWorldSubsystem, public ISavableObject
{
	GENERATED_BODY()

private:
	FTimerHandle ProcessDayTimer;

	FTimerHandle WidgetSpawnDelayTimerHandle;

	FRotator SunRotation, MoonRotation;

	UPROPERTY(Transient)
	ABaseLevelWorldSettings* BaseWorldSettings = nullptr;

	UPROPERTY(Transient)
	UDirectionalLightComponent* Sun = nullptr;
	
	UPROPERTY(Transient)
	UDirectionalLightComponent* Moon = nullptr;

	UPROPERTY(Transient)
	UExponentialHeightFogComponent* FogComponent = nullptr;

	UPROPERTY(Transient)
	USkyAtmosphereComponent* Sky = nullptr;
	
	UPROPERTY(Transient)
	UVolumetricCloudComponent* Cloud = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> CurrentWidget;
	
	bool hasDawnEnded, hasDuskStarted = false;

	float CurrentPhaseTimeInSeconds;

	float CurrentPhaseRotation, DawnRotation, DuskRotation, DayRotation;

	UPROPERTY(SaveGame)
	int DayCounter = 0;
	
	UPROPERTY(Transient)
	TMap<ULightComponent*, float> LightList;
	
	UPROPERTY(Transient)
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

	UFUNCTION()
	void SpawnNewDayPopUp();

	void OnPostLoad() override;

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
