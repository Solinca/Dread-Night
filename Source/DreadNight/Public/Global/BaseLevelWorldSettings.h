#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "Data/Wave/WaveDataAsset.h"
#include "BaseLevelWorldSettings.generated.h"

UCLASS()
class DREADNIGHT_API ABaseLevelWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UWorld> MapToLoadSubsystems = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Wave System")
	TArray<TObjectPtr<UWaveDataAsset>> WaveList;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Misc")
	TMap<FString, TObjectPtr<USoundBase>> SoundsToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Misc")
	TMap<FString, TSubclassOf<UUserWidget>> WidgetToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float DayTimeInSeconds = 900;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float DawnTimeInSeconds = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float DuskTimeInSeconds = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float StartSunRotation = 90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float FullSunRotation = 360;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float EndSunRotation = -270;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float DawnRotationThreshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float DuskRotationhreshold = -180;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Day Cycle System")
	float ProcessedTimeInterval = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	float SunIntensity = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	FLinearColor SunColor = FLinearColor(1.f, 1.f, 0.75f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	bool DoesSunCastShadow = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	float MoonIntensity = 0.075f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	FLinearColor MoonColor = FLinearColor(0.2f, 0.4f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Lights")
	bool DoesMoonCastShadow = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fog")
	float MaximalFogDensity = 0.025f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fog")
	float FogHeightFalloff = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fog")
	bool HasVolumetricFog = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fog", meta = (EditCondition = "HasVolumetricFog"))
	FLinearColor VolumetricFogAlbedo = FLinearColor(1.f, 0.5f, 0.6f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fog", meta = (EditCondition = "HasVolumetricFog"))
	float VolumetricFogExtinctionScale = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sky")
	float RayleighScatteringScale = 0.1f;
};
