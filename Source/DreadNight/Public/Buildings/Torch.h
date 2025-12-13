#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "Torch.generated.h"

UCLASS()
class DREADNIGHT_API ATorch : public ABuilding
{
	GENERATED_BODY()

protected:
	ATorch();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPointLightComponent> Light;
};
