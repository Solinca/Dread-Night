#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "Torch.generated.h"

UCLASS()
class DREADNIGHT_API ATorch : public AActor
{
	GENERATED_BODY()

protected:
	ATorch();
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPointLightComponent> Light = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> VFX = nullptr;
};
