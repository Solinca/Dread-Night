#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IA/DataAssets/MonsterDataAsset.h"
#include "IA/Characters/BaseAICharacter.h"
#include "Spawner.generated.h"

UCLASS()
class DREADNIGHT_API ASpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	ASpawner();

	virtual void BeginPlay() override;

public:
	ABaseAICharacter* SpawnCharacter(TSubclassOf<ABaseAICharacter> CharacterClass, UMonsterDataAsset* Data);
};
