// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BaseAIController.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogAIController, Log, Log)

UCLASS(Abstract, Blueprintable, BlueprintType, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior Tree")
	bool bAutoRunBehaviorTree{true};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior Tree")
	TObjectPtr<UBehaviorTree> UsedBehaviorTree;
	
public:
	// Sets default values for this actor's properties
	ABaseAIController();

	void TryRunBehaviorTree();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintNativeEvent, DisplayName = "SetupBlackboard")
	void BP_SetupBlackboard(UBlackboardComponent* BlackboardComponent);

	virtual void SetupBlackboard(UBlackboardComponent* BlackboardComponent) {}

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
	
	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnSightStimulusUpdated")
	void BP_OnSightStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude);

	virtual void OnSightStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude) {}

	UFUNCTION(BlueprintNativeEvent, DisplayName = "OnHearingStimulusUpdated")
	void BP_OnHearingStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude);

	virtual void OnHearingStimulusUpdated(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude) {}
	
#pragma endregion // Sense Callback
	
private:
	UFUNCTION()
	void InternalOnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
