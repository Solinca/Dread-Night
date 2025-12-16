#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UStaticMeshComponent;
class UMaterialInstance;

UCLASS()
class DREADNIGHT_API ABuilding : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* MatPlacementRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* MatPlacementGreen;

	bool bIsPlaced = false;

public:
	ABuilding();
	bool CheckValidPlacement();
	bool CheckIsOnGround();
	virtual void BeginPlay();
	virtual void PlaceBuilding();
	virtual void DestroyBuilding();
};