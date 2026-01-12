#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveSystem/SavableObject.h"
#include "ArmorComponent.generated.h"

class UArmorDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArmorChangedSignature, UArmorDataAsset*, ArmorData, bool, IsEquipped);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UArmorComponent : public UActorComponent, public ISavableObject
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

	UPROPERTY(SaveGame)
	float TotalDamageReduction = 0.f;

	UPROPERTY(SaveGame)
	TSoftObjectPtr<UArmorDataAsset> CurrentHelmetSave = nullptr;

	UPROPERTY(SaveGame)
	TSoftObjectPtr<UArmorDataAsset> CurrentChestPlateSave = nullptr;

	UPROPERTY(SaveGame)
	TSoftObjectPtr<UArmorDataAsset> CurrentPantsSave = nullptr;

	UPROPERTY(SaveGame)
	TSoftObjectPtr<UArmorDataAsset> CurrentBootsSave = nullptr;
protected:
	UArmorComponent();

public:
	FOnArmorChangedSignature OnArmorChanged;
	  
	void EquipArmor(UArmorDataAsset* Armor);

	float GetTotalDamageReduction() const { return TotalDamageReduction; }

	void OnPreSave();

	void OnPostLoad();
};
