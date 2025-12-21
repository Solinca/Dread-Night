#include "Actors/Traps/RotativeBladeTrap.h"
#include "Items/Data/BuildingDataAsset.h"
#include "GameFramework/RotatingMovementComponent.h"

ARotativeBladeTrap::ARotativeBladeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");
}

void ARotativeBladeTrap::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARotativeBladeTrap::OnBladeBeginOverlap);

	RotatingComponent->Deactivate();
}

void ARotativeBladeTrap::ActivateTrap()
{
	RotatingComponent->RotationRate = FRotator(0.f, TrapData->RotationSpeed, 0.f);

	RotatingComponent->Activate();
}

void ARotativeBladeTrap::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter * BaseAICharacter = Cast<ABaseAICharacter>(OtherActor))
	{
		BaseAICharacter->TryApplyDamage(TrapData->TrapDamage, this);
	}
}
