#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrapDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UTrapDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Variable")
	TObjectPtr<UStaticMesh> StaticMesh;

	/* Potential damage dealt to the monsters. */
	UPROPERTY(EditAnywhere, Category = "Variable")
	float TrapDamage = 0.f;

	/* Life span set after the trap has been activated. */
	UPROPERTY(EditAnywhere, Category = "Variable")
	float TrapLifeSpan = 0.f;

	UPROPERTY(EditAnywhere, Category = "Variable")
	bool bDoesRotate = false;

	UPROPERTY(EditAnywhere, Category = "Variable", meta = (EditCondition = "bDoesRotate"))
	float RotationSpeed = 360.f;
};
