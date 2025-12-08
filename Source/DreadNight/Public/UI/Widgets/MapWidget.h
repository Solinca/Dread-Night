// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSystem/MapDiscoverySubsystem.h"
#include "MapWidget.generated.h"

/**
 * Widget to display the global map with discovered locations
 */
UCLASS()
class DREADNIGHT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Map")
	void RefreshMap();

	UFUNCTION(BlueprintImplementableEvent, Category = "Map")
	void OnMapRefreshed(const TArray<FMapMarkerData>& Markers, const FVector& PlayerLocation);

	UFUNCTION(BlueprintCallable, Category = "Map")
	TArray<FMapMarkerData> GetDiscoveredMarkers() const;

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector GetPlayerLocation() const;

protected:
	UPROPERTY()
	TObjectPtr<UMapDiscoverySubsystem> MapDiscoverySubsystem;
};
