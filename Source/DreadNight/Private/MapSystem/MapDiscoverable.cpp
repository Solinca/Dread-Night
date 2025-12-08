// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSystem/MapDiscoverable.h"

// Default implementations
EMapMarkerType IMapDiscoverable::GetMarkerType_Implementation() const
{
	return EMapMarkerType::POI;
}

FString IMapDiscoverable::GetLocationName_Implementation() const
{
	return TEXT("Unknown Location");
}

FVector IMapDiscoverable::GetDiscoveryLocation_Implementation() const
{
	return FVector::ZeroVector;
}
