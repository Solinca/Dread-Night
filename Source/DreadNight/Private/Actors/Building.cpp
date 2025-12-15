#include "Actors/Building.h"
#include "Components/StaticMeshComponent.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(RootComponent);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

bool ABuilding::CheckValidPlacement()
{
	TArray<AActor*> OverlapingActors;
	MeshComp->GetOverlappingActors(OverlapingActors);

	MeshComp->SetOverlayMaterial(OverlapingActors.Num() == 0 ? MatPlacementGreen : MatPlacementRed);

	return OverlapingActors.Num() == 0;
}

void ABuilding::PlaceBuilding()
{
	if (bIsPlaced || !CheckValidPlacement()) return;

	bIsPlaced = true;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComp->SetOverlayMaterial(nullptr);
}

void ABuilding::DestroyBuilding()
{
	Destroy();
}