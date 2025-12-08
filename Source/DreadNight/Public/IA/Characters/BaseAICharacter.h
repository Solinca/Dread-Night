#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "IA/DataAssets/MonsterDataAsset.h"
#include "BaseAICharacter.generated.h"

/**
 * Base class for AI characters in the game, implementing the IGenericTeamAgentInterface.
 * Provides functionality for team-based AI behavior.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Category = "AI", ClassGroup = "AICharacter")
class DREADNIGHT_API ABaseAICharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UMonsterDataAsset> UsedDataAsset;
	
	TWeakObjectPtr<AAIController> AIController;
	
public:
	ABaseAICharacter();

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnDataAssetInitialization")
	void BP_OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset);

	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset) {}
};
