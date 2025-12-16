#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"


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

public:

	UFUNCTION(BlueprintCallable)
	void SetArmorDmgReductionMultiplier(float NewMultiplier);
	UFUNCTION(BlueprintCallable)
	void SetHelmetDmgReductionMultiplier(float NewMultiplier);

	float GetTotalDmgReductionMultiplier() const { return BaseDmgReductionMultiplier + (CurrentArmorDmgReductionMultiplier + CurrentHelmetDmgReductionMultiplier); }
};
