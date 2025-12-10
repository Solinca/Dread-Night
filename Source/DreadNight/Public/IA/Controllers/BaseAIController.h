#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BaseAIController.generated.h"



UCLASS(Abstract, Blueprintable, BlueprintType, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	/**
	 * Determines whether the behavior tree should automatically run on OnPossess.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior Tree")
	bool bAutoRunBehaviorTree{true};

public:
	ABaseAIController();

	/**
	 * Attempts to run the assigned behavior tree.
	 */
	void TryRunBehaviorTree();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

#pragma region AI Perception Base Callback
	
	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) {}

	UFUNCTION()
	virtual void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo) {}
	
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus& Stimulus) {}

	UFUNCTION()
	virtual void OnTargetPerceptionForgotten(AActor* Actor) {}
	
#pragma endregion // AI Perception Base Callback

#pragma region Senses Callback

	/**
	 * Blueprint-native event triggered when a sight stimulus is updated.
	 *
	 * @param Actor - The actor associated with the stimulus.
	 * @param Stimulus - The stimulus that was updated.
	 * @param TeamAttitude - The team attitude towards the actor.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnSightStimulusUpdated")
	void BP_OnSightStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude);

	/**
	 * Callback triggered when a sight stimulus is updated.
	 *
	 * @param Actor - The actor associated with the stimulus.
	 * @param Stimulus - The stimulus that was updated.
	 * @param TeamAttitude - The team attitude towards the actor.
	 */
	virtual void OnSightStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude) {}

	/**
	 * Blueprint-native event triggered when a hearing stimulus is updated.
	 *
	 * @param Actor - The actor associated with the stimulus.
	 * @param Stimulus - The stimulus that was updated.
	 * @param TeamAttitude - The team attitude towards the actor.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnHearingStimulusUpdated")
	void BP_OnHearingStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude);

	/**
	 * Callback triggered when a hearing stimulus is updated.
	 *
	 * @param Actor - The actor associated with the stimulus.
	 * @param Stimulus - The stimulus that was updated.
	 * @param TeamAttitude - The team attitude towards the actor.
	 */
	virtual void OnHearingStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude) {}
	
#pragma endregion // Sense Callback

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
private:
	/**
	 * Internal implementation to be call in the OnPossess function.
	 */
	void TryRunBehaviorTree(APawn* InPawn);
	
	UFUNCTION()
	void InternalOnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
