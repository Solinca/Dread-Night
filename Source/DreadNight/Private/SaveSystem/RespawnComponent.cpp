// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/RespawnComponent.h"

#include "Subsystems/World/DayCycleSubSystem.h"


// Sets default values for this component's properties
URespawnComponent::URespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->OnDawnStart.AddDynamic(this, &ThisClass::URespawnComponent::OnNewDayUpdate);
	
}


void URespawnComponent::OnNewDayUpdate()
{
	NightBeforeRespawn--;
	if (bIsDead && NightBeforeRespawn <= 0)
	{
		Respawn();
	}
}

void URespawnComponent::Despawn()
{
	NightBeforeRespawn = RespawnNightNumber;
	bIsDead = true;
	AActor* Owner = GetOwner();
	Owner->SetActorEnableCollision(false);
	Owner->SetActorHiddenInGame(true);
}

void URespawnComponent::Respawn()
{
	bIsDead = false;
	AActor* Owner = GetOwner();
	Owner->SetActorEnableCollision(true);
	Owner->SetActorHiddenInGame(false);
	OnRespawn.ExecuteIfBound();
}

void URespawnComponent::OnPostLoad()
{
	if (!bIsDead)
	{
		Respawn();
	}
	else
	{
		Despawn();
	}
	
	++NightBeforeRespawn;
}

