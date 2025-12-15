#include "Actors/Traps/RotativeBladeTrap.h"

ARotativeBladeTrap::ARotativeBladeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");

	RotatingComponent->Deactivate();

}


void ARotativeBladeTrap::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetStaticMesh(TrapData->StaticMesh);

	MeshComp->SetCollisionProfileName("Trap");

	RotatingComponent->RotationRate = FRotator(0.f, TrapData->RotationSpeed, 0.f);


	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARotativeBladeTrap::OnBladeBeginOverlap);
}

void ARotativeBladeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		RotatingComponent->Activate();
	}
	else 
	{
		RotatingComponent->Deactivate();
	}
}

void ARotativeBladeTrap::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter * BaseAICharacter{ Cast<ABaseAICharacter>(OtherActor) })
	{
		BaseAICharacter->TryApplyDamage(TrapData->TrapDamage, this);
	}
}
