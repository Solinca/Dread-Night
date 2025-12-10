#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "WeaponDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UWeaponDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float Cooldown;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> WeaponMesh;
};
