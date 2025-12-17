// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Object/ItemInstance.h"
#include "Items/Interface/UsableItem.h"
#include "Items/Data/BuildingDataAsset.h"
#include "ItemInstance_Building.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UItemInstance_Building : public UItemInstance, public IUsableItem
{
public:
	virtual FName GetActionName() override;
	virtual void Use(AActor* Player) override;
	UBuildingDataAsset* GetDataAsset();

private:
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	UBuildingDataAsset* BuildingDataAsset;

	virtual void OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack) override;
	
};
