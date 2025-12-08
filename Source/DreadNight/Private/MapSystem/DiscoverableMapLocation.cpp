// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSystem/DiscoverableMapLocation.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ADiscoverableMapLocation::ADiscoverableMapLocation()
{
	PrimaryActorTick.bCanEverTick = true;

	LocationType = EMapMarkerType::POI;
	LocationName = TEXT("Unknown Location");
	DiscoveryRadius = 1000.0f;
	bDiscoverOnBeginPlay = false;
	bHasBeenDiscovered = false;
}

void ADiscoverableMapLocation::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		MapDiscoverySubsystem = World->GetSubsystem<UMapDiscoverySubsystem>();
	}

	if (bDiscoverOnBeginPlay)
	{
		DiscoverLocation();
	}
}

void ADiscoverableMapLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if player is within discovery radius
	if (!bHasBeenDiscovered && MapDiscoverySubsystem)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (APawn* PlayerPawn = PC->GetPawn())
			{
				float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
				if (Distance <= DiscoveryRadius)
				{
					DiscoverLocation();
				}
			}
		}
	}
}

void ADiscoverableMapLocation::DiscoverLocation()
{
	if (bHasBeenDiscovered || !MapDiscoverySubsystem)
	{
		return;
	}

	MapDiscoverySubsystem->DiscoverLocation(GetActorLocation(), LocationType, LocationName);
	bHasBeenDiscovered = true;

	// Optionally disable tick after discovery to save performance
	SetActorTickEnabled(false);
}
