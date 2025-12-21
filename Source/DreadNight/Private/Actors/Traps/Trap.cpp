#include "Actors/Traps/Trap.h"
#include "Components/BoxComponent.h"
#include "Items/Data/BuildingDataAsset.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");

	TrapCollisionBox = CreateDefaultSubobject<UBoxComponent>("Trap Collision Component");
	
	TrapCollisionBox->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	Mesh->SetupAttachment(RootComponent);
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();

	TrapCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBeginOverlap);
}

void ATrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseAICharacter* BaseAICharacter = Cast<ABaseAICharacter>(OtherActor);

	if (!TimerHandle_LifeSpanExpired.IsValid() && BaseAICharacter)
	{
		SetLifeSpan(TrapData->TrapLifeSpan);

		ActivateTrap();
	}
}
