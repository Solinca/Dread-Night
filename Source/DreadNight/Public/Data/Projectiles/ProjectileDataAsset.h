#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UProjectileDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	float Damage = 10.f;
};
