#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "Data/Wave/WaveSystemDataAsset.h"
#include "Data/DayCycleSystem/DayCycleSystemDataAsset.h"
#include "BaseLevelWorldSettings.generated.h"

UCLASS()
class DREADNIGHT_API ABaseLevelWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UWorld> MapToLoadSubsystems = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UWaveSystemDataAsset> WaveSystemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UDayCycleSystemDataAsset> DayCycleSystemData;
};
