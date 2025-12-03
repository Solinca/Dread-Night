// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "WarriorAICharacter.generated.h"

UCLASS()
class DREADNIGHT_API AWarriorAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarriorAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
