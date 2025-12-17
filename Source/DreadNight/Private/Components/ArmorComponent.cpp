#include "Components/ArmorComponent.h"

#include "Items/Data/ArmorDataAsset.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArmorComponent::EquipArmor(UArmorDataAsset* Armor)
{	
	ArmorDataAsset = Armor;
	CurrentArmorDmgReductionMultiplier = Armor ? Armor->DamageReductionMultiplier : 0;
	ArmorMesh->SetStaticMesh(Armor ? Armor->ArmorMesh : nullptr);
	OnArmorEquipped.Broadcast(Armor);
}

void UArmorComponent::EquipHelmet(UArmorDataAsset* Helmet)
{
	HelmetDataAsset = Helmet;
	CurrentHelmetDmgReductionMultiplier = Helmet ? Helmet->DamageReductionMultiplier : 0;
	HelmetMesh->SetStaticMesh(Helmet ? Helmet->ArmorMesh : nullptr);
	OnArmorEquipped.Broadcast(Helmet);
}

void UArmorComponent::SetupMesh(UStaticMeshComponent* Helmet, UStaticMeshComponent* Armor)
{
	HelmetMesh = Helmet;
	ArmorMesh = Armor;
}
