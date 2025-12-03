// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "WarriorAIController.generated.h"

UCLASS()
class DREADNIGHT_API AWarriorAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarriorAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
