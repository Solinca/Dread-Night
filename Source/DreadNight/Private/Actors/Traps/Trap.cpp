// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Traps/Trap.h"

ATrap::ATrap()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = true;

	TrapCollisionComponent = CreateDefaultSubobject<UBoxComponent>("Trap Collision Component");
	TrapCollisionComponent->SetCollisionProfileName("Trap");
	TrapCollisionComponent->SetupAttachment(RootComponent);
}

void ATrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	OnTrapTick(DeltaSeconds);

	CheckToFinishTrapTicking();
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
		TrapTickCurrentCount = 0;
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
		OnTrapTrigger(BaseAICharacter);
	}
}
