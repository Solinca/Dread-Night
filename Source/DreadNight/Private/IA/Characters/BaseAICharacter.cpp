// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Characters/BaseAICharacter.h"

#include "AIController.h"


// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseAICharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	Cast<AAIController>(Controller)->SetGenericTeamId(TeamID);
}

FGenericTeamId ABaseAICharacter::GetGenericTeamId() const
{
	return Cast<AAIController>(Controller)->GetGenericTeamId();
}

ETeamAttitude::Type ABaseAICharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Cast<AAIController>(Controller)->GetTeamAttitudeTowards(Other);
}
