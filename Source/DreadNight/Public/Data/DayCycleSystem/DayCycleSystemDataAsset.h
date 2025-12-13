#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DayCycleSystemDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UDayCycleSystemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float DayTimeInSeconds = 900;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float DawnTimeInSeconds = 10;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float DuskTimeInSeconds = 10;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float StartSunRotation = 90;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float FullSunRotation = 360;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float EndSunRotation = -270;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float DawnRotationThreshold = 0;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float DuskRotationhreshold = -180;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float ProcessedTimeInterval = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Global Settings")
	float SunIntensity = 10.f;

	UPROPERTY(EditAnywhere, Category = "Lights")
	FLinearColor SunColor = FLinearColor(1.f, 1.f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "Lights")
	bool DoesSunCastShadow = true;

	UPROPERTY(EditAnywhere, Category = "Lights")
	float MoonIntensity = 0.075f;

	UPROPERTY(EditAnywhere, Category = "Lights")
	FLinearColor MoonColor = FLinearColor(0.2f, 0.4f, 1.f);

	UPROPERTY(EditAnywhere, Category = "Lights")
	bool DoesMoonCastShadow = false;

	UPROPERTY(EditAnywhere, Category = "Fog")
	float MaximalFogDensity = 0.025f;

	UPROPERTY(EditAnywhere, Category = "Fog")
	float FogHeightFalloff = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Fog")
	bool HasVolumetricFog = true;

	UPROPERTY(EditAnywhere, Category = "Fog", meta = (EditCondition = "HasVolumetricFog"))
	FLinearColor VolumetricFogAlbedo = FLinearColor(1.f, 0.5f, 0.6f);

	UPROPERTY(EditAnywhere, Category = "Fog", meta = (EditCondition = "HasVolumetricFog"))
	float VolumetricFogExtinctionScale = 5.f;

	UPROPERTY(EditAnywhere, Category = "Sky")
	float RayleighScatteringScale = 0.1f;

	UPROPERTY(EditAnywhere, Category = "UI")
	TMap<FString, TSubclassOf<UUserWidget>> WidgetToSpawn;

	UPROPERTY(EditAnywhere, Category = "UI")
	float WidgetSpawnDelay = 1.f;
};
