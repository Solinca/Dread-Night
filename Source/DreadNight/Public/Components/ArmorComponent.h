#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"

class UArmorDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArmorChangedSignature, UArmorDataAsset*, ArmorData, bool, IsEquipped);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TObjectPtr<UArmorDataAsset> CurrentHelmet = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UArmorDataAsset> CurrentChestPlate = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UArmorDataAsset> CurrentPants = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UArmorDataAsset> CurrentBoots = nullptr;

	float TotalDamageReduction = 0.f;

protected:
	UArmorComponent();

public:
	FOnArmorChangedSignature OnArmorChanged;
	  
	void EquipArmor(UArmorDataAsset* Armor);

	float GetTotalDamageReduction() const { return TotalDamageReduction; }
};
