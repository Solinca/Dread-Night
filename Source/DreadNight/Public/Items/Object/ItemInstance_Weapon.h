#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "Items/Interface/UsableItem.h"
#include "ItemInstance_Weapon.generated.h"


class UWeaponDataAsset;

UCLASS()
class DREADNIGHT_API UItemInstance_Weapon : public UItemInstance, public IUsableItem
{
public:
	virtual FName GetActionName() override;
	virtual void Use(AActor* Player) override;
	
private:
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	UWeaponDataAsset* WeaponDataAsset;


	virtual void OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack) override;
};
