// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Object/ItemInstance_Armor.h"
#include "ItemInstance_ChestArmor.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UItemInstance_ChestArmor : public UItemInstance_Armor
{
	GENERATED_BODY()
protected:
	virtual void Use(AActor* Player) override;
};
