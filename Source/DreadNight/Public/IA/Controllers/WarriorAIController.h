// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "WarriorAIController.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API AWarriorAIController : public ABaseAIController
{
	GENERATED_BODY()

	FTimerHandle RetrievePlayerCharacterTimerHandle;
	
protected:
	virtual void SetupBlackboard(UBlackboardComponent* BlackboardComponent) override;
};
