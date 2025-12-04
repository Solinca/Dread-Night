#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

/**
 * Base class for AI characters in the game, implementing the IGenericTeamAgentInterface.
 * Provides functionality for team-based AI behavior.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ABaseAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
