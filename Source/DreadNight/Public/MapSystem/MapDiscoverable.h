// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MapSystem/MapDiscoverySubsystem.h"
#include "MapDiscoverable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UMapDiscoverable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can be discovered on the map
 */
class DREADNIGHT_API IMapDiscoverable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Map Discovery")
	EMapMarkerType GetMarkerType() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Map Discovery")
	FString GetLocationName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Map Discovery")
	FVector GetDiscoveryLocation() const;
};
