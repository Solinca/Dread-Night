// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExcludeBase.h"
#include "Components/BoxComponent.h"
#include "UObject/Object.h"
#include "ExcludeZone.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API AExcludeZone : public AExcludeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BlockingZone;

	AExcludeZone();
};
