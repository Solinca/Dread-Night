#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"


class UArmorDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorEquipped, UArmorDataAsset*, ArmorData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UArmorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 0.f))
	float BaseDmgReductionMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 0.f))
	float CurrentArmorDmgReductionMultiplier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 0.f))
	float CurrentHelmetDmgReductionMultiplier = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMeshComponent> HelmetMesh;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMeshComponent> ArmorMesh;
	
	UPROPERTY()
	TObjectPtr<UArmorDataAsset> HelmetDataAsset;
	
	UPROPERTY()
	TObjectPtr<UArmorDataAsset> ArmorDataAsset;
public:

	FOnArmorEquipped OnArmorEquipped;
	  
	UFUNCTION(BlueprintCallable)
	void EquipArmor(UArmorDataAsset* Armor);
	
	UFUNCTION(BlueprintCallable)
	void EquipHelmet(UArmorDataAsset* Helmet);

	void SetupMesh(UStaticMeshComponent* Helmet,UStaticMeshComponent* Armor);
	
	float GetTotalDmgReductionMultiplier() const { return BaseDmgReductionMultiplier + (CurrentArmorDmgReductionMultiplier + CurrentHelmetDmgReductionMultiplier); }
};
