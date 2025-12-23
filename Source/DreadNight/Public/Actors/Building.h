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

private:
	FHitResult Hit;

	FTimerHandle TickTimerHandle;

	TObjectPtr<class UBuildingDataAsset> BuildingDataAsset = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Material")
	TObjectPtr<UMaterialInstance> MatPlacementRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Material")
	TObjectPtr<UMaterialInstance> MatPlacementGreen;

public:
	ABuilding();

	bool CheckValidPlacement();

	bool CheckIsOnGround();

	virtual void PlaceBuilding();

	void SetDataAsset(UBuildingDataAsset* DataAsset);
	
	UBuildingDataAsset* GetDataAsset() const { return BuildingDataAsset; }
	UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }
};