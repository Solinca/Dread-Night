#pragma once

#include "CoreMinimal.h"
#include "Items/Data/ItemDataAsset.h"
#include "Actors/Building.h"
#include "BuildingDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UBuildingDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<AActor> BuildingToSpawn;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TObjectPtr<UStaticMesh> BuildingPreviewMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FRotator BuildingPreviewRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector BuildingPreviewScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxDistanceFromGround = 100.f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool IsTrap = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "IsTrap"))
	float TrapDamage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "IsTrap"))
	float TrapLifeSpan = 0.f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "IsTrap"))
	bool DoesRotate = false;

	UPROPERTY(EditAnywhere, Category = "Variable", meta = (EditCondition = "DoesRotate"))
	float RotationSpeed = 360.f;
};
