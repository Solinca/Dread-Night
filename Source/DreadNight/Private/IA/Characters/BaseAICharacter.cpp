#include "IA/Characters/BaseAICharacter.h"

#include "AIController.h"

ABaseAICharacter::ABaseAICharacter()
{
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
