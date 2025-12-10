// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "UObject/Object.h"
#include "MyClass.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API AExcludeZone : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BlockingZone;

	AExcludeZone();
};
