#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IA/DataAssets/MonsterDataAsset.h"
#include "IA/Characters/BaseAICharacter.h"
#include "WaveSystemDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FWaveMonsterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMonsterDataAsset> Data = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseAICharacter> CharacterClass = nullptr;

	UPROPERTY(EditAnywhere)
	int Count = 0;
};

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWaveMonsterData> MonsterList;
};

UCLASS()
class DREADNIGHT_API UWaveSystemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FWaveData> WaveList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> NewWaveSound;
};
