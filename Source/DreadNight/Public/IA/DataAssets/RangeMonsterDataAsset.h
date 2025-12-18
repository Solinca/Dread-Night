#pragma once

#include "CoreMinimal.h"
#include "HostileMonsterDataAsset.h"
#include "Actors/ProjectileActor.h"
#include "RangeMonsterDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API URangeMonsterDataAsset : public UHostileMonsterDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileActor> SpawnedProjectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThrowBoneName;

public:
	[[nodiscard]] TSubclassOf<AProjectileActor> GetSpawnedProjectile() const;
	[[nodiscard]] float GetFleeRange() const;
	[[nodiscard]] const FName& GetThrowBoneName() const;
};
