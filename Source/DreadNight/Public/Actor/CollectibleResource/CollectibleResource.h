// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleResource.generated.h"

class UItemDataAsset;

UCLASS()
class DREADNIGHT_API ACollectibleResource : public AActor
{
	GENERATED_BODY()

public: 
	ACollectibleResource();

protected: 
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	const TObjectPtr<UItemDataAsset> ItemDataAsset;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ItemQuantityRange;

	int CurrentItemQuantity; 
	
public:

	
};
