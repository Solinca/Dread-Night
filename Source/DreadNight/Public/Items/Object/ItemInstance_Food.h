#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "Items/Interface/UsableItem.h"
#include "ItemInstance_Food.generated.h"


class UFoodDataAsset;

UCLASS()
class DREADNIGHT_API UItemInstance_Food : public UItemInstance, public IUsableItem
{
public:
	virtual FName GetActionName() override;
	virtual void Use(AActor* Player) override;
	
private:
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	UFoodDataAsset* FoodDataAsset;


	virtual void OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack) override;
};
