// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MapDiscoverySubsystem.generated.h"

UENUM(BlueprintType)
enum class EMapMarkerType : uint8
{
	POI UMETA(DisplayName = "Point of Interest"),
	Gatherable UMETA(DisplayName = "Gatherable Resource"),
	Player UMETA(DisplayName = "Player Position")
};

USTRUCT(BlueprintType)
struct FMapMarkerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector WorldLocation;

	UPROPERTY(BlueprintReadWrite)
	EMapMarkerType MarkerType;

	UPROPERTY(BlueprintReadWrite)
	FString MarkerName;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDiscovered;

	FMapMarkerData()
		: WorldLocation(FVector::ZeroVector)
		, MarkerType(EMapMarkerType::POI)
		, MarkerName(TEXT(""))
		, bIsDiscovered(false)
	{}
};

/**
 * World subsystem to track discovered map locations
 */
UCLASS()
class DREADNIGHT_API UMapDiscoverySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Map")
	void DiscoverLocation(const FVector& WorldLocation, EMapMarkerType MarkerType, const FString& MarkerName);

	UFUNCTION(BlueprintCallable, Category = "Map")
	TArray<FMapMarkerData> GetDiscoveredMarkers() const;

	UFUNCTION(BlueprintCallable, Category = "Map")
	void ClearDiscoveredMarkers();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void DiscoverActor(AActor* Actor);

protected:
	UPROPERTY()
	TArray<FMapMarkerData> DiscoveredMarkers;
};
