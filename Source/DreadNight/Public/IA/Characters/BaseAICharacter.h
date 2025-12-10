#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "IA/DataAssets/MonsterDataAsset.h"
#include "Components/HealthComponent.h"
#include "DamageSystem/Interface/Damageable.h"
#include "BaseAICharacter.generated.h"

/**
 * Base class for AI characters in the game, implementing the IGenericTeamAgentInterface.
 * Provides functionality for team-based AI behavior.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Category = "AI", ClassGroup = "AICharacter")
class DREADNIGHT_API ABaseAICharacter : public ACharacter, public IGenericTeamAgentInterface, public IDamageable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UMonsterDataAsset> UsedDataAsset;
	
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;
	
public:
	ABaseAICharacter();

	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	virtual void PossessedBy(AController* NewController) override;

	UMonsterDataAsset* GetMonsterData() const;
protected:
	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnDataAssetInitialization")
	void BP_OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset);

	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset);
};
