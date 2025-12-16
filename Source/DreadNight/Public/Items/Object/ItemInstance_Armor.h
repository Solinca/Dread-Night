#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "Items/Interface/UsableItem.h"
#include "ItemInstance_Armor.generated.h"


class UArmorDataAsset;

UCLASS()
class DREADNIGHT_API UItemInstance_Armor : public UItemInstance, public IUsableItem
{
public:
	virtual FName GetActionName() override;
	virtual void Use(AActor* Player) PURE_VIRTUAL(UItemInstance_Armor::Use);
	UArmorDataAsset* GetDataAsset();

private:
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	UArmorDataAsset* ArmorDataAsset;

	virtual void OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack) override;
};
