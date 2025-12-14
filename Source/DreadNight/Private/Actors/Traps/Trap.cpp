// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/Trap.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = false;

	TrapCollisionComponent = CreateDefaultSubobject<UBoxComponent>("Trap Collision Component");
	TrapCollisionComponent->SetCollisionProfileName("Trap");
	TrapCollisionComponent->SetupAttachment(RootComponent);
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();

	TrapCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBeginOverlap);
}

void ATrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(TickTimerHandle);
}

void ATrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter* BaseAICharacter{Cast<ABaseAICharacter>(OtherActor)})
	{
		SetLifeSpan(TrapLifeSpan);
		GetWorldTimerManager().SetTimer(TickTimerHandle, this, &ATrap::OnTrapTick, PrimaryActorTick.TickInterval, true);
	}
}
