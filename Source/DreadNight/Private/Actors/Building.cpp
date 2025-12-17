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

	if (bNetStartup) bIsPlaced = true;
	else
	{
		MeshComp->SetGenerateOverlapEvents(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	}

}

bool ABuilding::CheckValidPlacement()
{

	TArray<AActor*> OverlapingActors;
	MeshComp->GetOverlappingActors(OverlapingActors);

	bool bIsValid = (OverlapingActors.Num() == 0 && CheckIsOnGround());

	MeshComp->SetOverlayMaterial(bIsValid ? MatPlacementGreen : MatPlacementRed);

	return bIsValid;

}

bool ABuilding::CheckIsOnGround()
{

	FHitResult Hit;

	return GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		GetActorLocation() - FVector(0, 0, 100.f),
		ECC_Visibility
	);

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

	if(!bNetStartup) Destroy();

}