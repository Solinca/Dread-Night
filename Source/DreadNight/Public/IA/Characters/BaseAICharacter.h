// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ABaseAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
