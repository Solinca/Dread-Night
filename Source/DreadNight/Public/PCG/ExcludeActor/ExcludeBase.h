// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExcludeBase.generated.h"

UCLASS()
class DREADNIGHT_API AExcludeBase : public AActor
{
	GENERATED_BODY()

protected: 
	UPROPERTY(EditAnywhere)
	bool bAffectGrassToo = false;

	AExcludeBase();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
