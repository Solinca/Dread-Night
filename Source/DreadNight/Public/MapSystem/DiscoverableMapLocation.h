// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapSystem/MapDiscoverySubsystem.h"
#include "DiscoverableMapLocation.generated.h"

/**
 * Actor that registers itself with the map discovery system when player gets near
 */
UCLASS()
class DREADNIGHT_API ADiscoverableMapLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	ADiscoverableMapLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Discovery")
	EMapMarkerType LocationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Discovery")
	FString LocationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Discovery")
	float DiscoveryRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Discovery")
	bool bDiscoverOnBeginPlay;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Map Discovery")
	void DiscoverLocation();

private:
	bool bHasBeenDiscovered;

	UPROPERTY()
	TObjectPtr<UMapDiscoverySubsystem> MapDiscoverySubsystem;
};
