// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSystem/MapDiscoverySubsystem.h"

void UMapDiscoverySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DiscoveredMarkers.Empty();
}

void UMapDiscoverySubsystem::Deinitialize()
{
	Super::Deinitialize();
	DiscoveredMarkers.Empty();
}

void UMapDiscoverySubsystem::DiscoverLocation(const FVector& WorldLocation, EMapMarkerType MarkerType, const FString& MarkerName)
{
	// Check if this location is already discovered (within a certain distance)
	const float DiscoveryRadius = 100.0f;
	for (const FMapMarkerData& Marker : DiscoveredMarkers)
	{
		if (Marker.MarkerType == MarkerType && 
			FVector::Dist(Marker.WorldLocation, WorldLocation) < DiscoveryRadius)
		{
			// Already discovered
			return;
		}
	}

	// Add new discovered location
	FMapMarkerData NewMarker;
	NewMarker.WorldLocation = WorldLocation;
	NewMarker.MarkerType = MarkerType;
	NewMarker.MarkerName = MarkerName;
	NewMarker.bIsDiscovered = true;

	DiscoveredMarkers.Add(NewMarker);
}

TArray<FMapMarkerData> UMapDiscoverySubsystem::GetDiscoveredMarkers() const
{
	return DiscoveredMarkers;
}

void UMapDiscoverySubsystem::ClearDiscoveredMarkers()
{
	DiscoveredMarkers.Empty();
}
