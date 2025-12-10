#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IA/DataAssets/MonsterDataAsset.h"
#include "IA/Characters/BaseAICharacter.h"
#include "WaveDataAsset.generated.h"

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

UCLASS()
class DREADNIGHT_API UWaveDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FWaveMonsterData> MonsterList;
};
