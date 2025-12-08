// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/MapWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		MapDiscoverySubsystem = World->GetSubsystem<UMapDiscoverySubsystem>();
	}

	RefreshMap();
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// Optionally refresh the player position each tick
	// RefreshMap();
}

void UMapWidget::RefreshMap()
{
	if (!MapDiscoverySubsystem)
	{
		return;
	}

	TArray<FMapMarkerData> DiscoveredMarkers = MapDiscoverySubsystem->GetDiscoveredMarkers();
	FVector PlayerLocation = GetPlayerLocation();

	OnMapRefreshed(DiscoveredMarkers, PlayerLocation);
}

TArray<FMapMarkerData> UMapWidget::GetDiscoveredMarkers() const
{
	if (MapDiscoverySubsystem)
	{
		return MapDiscoverySubsystem->GetDiscoveredMarkers();
	}
	return TArray<FMapMarkerData>();
}

FVector UMapWidget::GetPlayerLocation() const
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			return PlayerPawn->GetActorLocation();
		}
	}
	return FVector::ZeroVector;
}
