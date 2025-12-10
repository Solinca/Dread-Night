#include "Actors/Building.h"
#include "Components/StaticMeshComponent.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(RootComponent);
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

bool ABuilding::CheckValidPlacement()
{
	TArray<AActor*> OverlapingActors;
	MeshComp->GetOverlappingActors(OverlapingActors);

	return OverlapingActors.Num() == 0;
}

void ABuilding::PlaceBuilding()
{
	if (bIsPlaced || !CheckValidPlacement()) return;

	bIsPlaced = true;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
}

void ABuilding::DestroyBuilding()
{
	Destroy();
}