#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UStaticMeshComponent;

UCLASS()
class DREADNIGHT_API ABuilding : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;

	bool bIsPlaced = false;

public:
	ABuilding();
	bool CheckValidPlacement();
	virtual void PlaceBuilding();
	virtual void DestroyBuilding();
};