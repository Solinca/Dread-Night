#include "Items/Object/ItemInstance_Weapon.h"

#include "Items/Data/WeaponDataAsset.h"
#include "Components/SwordCombatComponent.h"
#include "Components/BowCombatComponent.h"

FName UItemInstance_Weapon::GetActionName()
{
	return FName(TEXT("Equip"));
}

void UItemInstance_Weapon::Use(AActor* Player)
{
	if (USwordCombatComponent* SwordComponent = Player->GetComponentByClass<USwordCombatComponent>())
	{
		if (WeaponDataAsset->Type.GetTagName().ToString().Contains("Item.Weapon.Sword"))
			SwordComponent->SetWeapon(WeaponDataAsset);
	}
	if (UBowCombatComponent* BowComponent = Player->GetComponentByClass<UBowCombatComponent>())
	{
		if (WeaponDataAsset->Type.GetTagName().ToString().Contains("Item.Weapon.Bow"))
			BowComponent->SetWeapon(WeaponDataAsset);
	}
}

UWeaponDataAsset* UItemInstance_Weapon::GetDataAsset()
{
	return WeaponDataAsset;
}

void UItemInstance_Weapon::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (WeaponDataAsset = Cast<UWeaponDataAsset>(DataAsset); !WeaponDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
