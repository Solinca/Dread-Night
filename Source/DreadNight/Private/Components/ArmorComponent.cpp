#include "Components/ArmorComponent.h"

#include "Items/Data/ArmorDataAsset.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArmorComponent::EquipArmor(UArmorDataAsset* Armor)
{	
	CurrentArmorDmgReductionMultiplier = Armor ? Armor->DamageReductionMultiplier : 0;
	ArmorMesh->SetStaticMesh(Armor ? Armor->ArmorMesh : nullptr);
}

void UArmorComponent::EquipHelmet(UArmorDataAsset* Helmet)
{
	CurrentHelmetDmgReductionMultiplier = Helmet ? Helmet->DamageReductionMultiplier : 0;
	HelmetMesh->SetStaticMesh(Helmet ? Helmet->ArmorMesh : nullptr);
}

void UArmorComponent::SetupMesh(UStaticMeshComponent* Helmet, UStaticMeshComponent* Armor)
{
	HelmetMesh = Helmet;
	ArmorMesh = Armor;
}
