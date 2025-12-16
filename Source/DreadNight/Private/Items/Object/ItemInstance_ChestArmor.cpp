// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Object/ItemInstance_ChestArmor.h"

#include "Components/ArmorComponent.h"

void UItemInstance_ChestArmor::Use(AActor* Player)
{
	if (!Player)
		return;

	if (UArmorComponent* ArmorComponent = Player->GetComponentByClass<UArmorComponent>())
	{
		ArmorComponent->EquipArmor(ArmorDataAsset);
	}
}
