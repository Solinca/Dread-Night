#include "Actors/Building.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Data/BuildingDataAsset.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
}

bool ABuilding::CheckValidPlacement()
{
	TArray<AActor*> OverlapingActors;

	MeshComp->GetOverlappingActors(OverlapingActors);

	bool bIsValid = (OverlapingActors.Num() == 0 && CheckIsOnGround());

	for (int i = 0; i < MeshComp->GetNumMaterials(); i++)
	{
		MeshComp->SetMaterial(i, bIsValid ? MatPlacementGreen : MatPlacementRed);
	}

	return bIsValid;
}

bool ABuilding::CheckIsOnGround()
{
	FBox Box = MeshComp->CalcBounds(MeshComp->GetComponentTransform()).GetBox();
	return GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(0,0, GetActorLocation().Z - Box.Min.Z), GetActorLocation() - FVector(0, 0, BuildingDataAsset->MaxDistanceFromGround + GetActorLocation().Z - Box.Min.Z), ECC_Visibility);
}

void ABuilding::PlaceBuilding()
{
	GetWorld()->SpawnActor<AActor>(BuildingDataAsset->BuildingToSpawn, GetActorLocation(), FRotator(0, 0, 0));

	Destroy();
}

void ABuilding::SetDataAsset(UBuildingDataAsset* DataAsset)
{
	BuildingDataAsset = DataAsset;

	MeshComp->SetStaticMesh(BuildingDataAsset->BuildingPreviewMesh);

	SetActorRotation(BuildingDataAsset->BuildingPreviewRotation);

	SetActorScale3D(BuildingDataAsset->BuildingPreviewScale);

	CheckValidPlacement();
}
