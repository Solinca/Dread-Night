// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/RotativeBladeTrap.h"

ARotativeBladeTrap::ARotativeBladeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	TrapLifeSpan = 5.f;
	TrapDamage = 10.f;	

	TrapBlade = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	TrapBlade->SetupAttachment(RootComponent);
	TrapBlade->SetCollisionProfileName("Trap");

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");
	RotatingComponent->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
	RotatingComponent->Deactivate();
}


void ARotativeBladeTrap::BeginPlay()
{
	Super::BeginPlay();

	TrapBlade->OnComponentBeginOverlap.AddDynamic(this, &ARotativeBladeTrap::OnBladeBeginOverlap);
}

void ARotativeBladeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)//Trap has been Activated
		RotatingComponent->Activate();
	else
		RotatingComponent->Deactivate();
}

void ARotativeBladeTrap::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter * BaseAICharacter{ Cast<ABaseAICharacter>(OtherActor) })
	{
		//Apply damage
		BaseAICharacter->TryApplyDamage(TrapDamage, this);
	}
}
