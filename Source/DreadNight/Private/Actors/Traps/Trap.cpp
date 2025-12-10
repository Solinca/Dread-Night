// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/Trap.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = true;

	TrapCollisionComponent = CreateDefaultSubobject<UBoxComponent>("Trap Collision Component");
	TrapCollisionComponent->SetCollisionProfileName("Trap");
}

void ATrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	OnTrapTick();

	
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

void ATrap::StartTrapTicking()
{
	SetActorTickEnabled(true);
	
	if (!bTrapIsUsingTimer || bTrapTickInfinitely)
	{
		return;
	}
	
	GetWorldTimerManager().SetTimer(TickTimerHandle, [this]
	{
		StopTrapTicking();
	}, TrapTimer, false);
}

void ATrap::StopTrapTicking()
{
	OnTrapTickFinish();
	SetActorTickEnabled(false);
}

void ATrap::BP_OnTrapTrigger_Implementation(ABaseAICharacter* TrapTriggerInvestigator)
{
	OnTrapTrigger(TrapTriggerInvestigator);
}

void ATrap::CheckToFinishTrapTicking()
{
	if (bTrapIsUsingTimer || bTrapTickInfinitely)
	{
		return;
	}
	
	if (++TrapTickCurrentCount >= TrapTickCount)
	{
		StopTrapTicking();
	}
}

void ATrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter* BaseAICharacter{Cast<ABaseAICharacter>(OtherActor)})
	{
		BP_OnTrapTrigger(BaseAICharacter);
	}
}
