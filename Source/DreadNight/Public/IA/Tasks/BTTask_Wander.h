#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BTTask_Base.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "IA/Tasks/AITask_Wander.h"
#include "BTTask_Wander.generated.h"

/**
 * @struct FBTWanderTaskMemory
 * @brief Holds the runtime state for an instance of the UBTTask_Wander node.
 */
struct FBTWanderTaskMemory
{
	TVariant<UObject*, bool> WanderStop;
	
	TWeakObjectPtr<UAITask_Wander> Task;
	TWeakObjectPtr<UEnvQuery> WanderQuery;

	float AcceptableRadius{0.f};
	float WanderWaitTime{0.f};

	bool bInvertedCondition{false};
	bool bIsInitialized{false};

	FBTWanderTaskMemory() = default;
	~FBTWanderTaskMemory() = default;

	// Resets the memory to its default state.
	void CleanMemory();
};

/**
 * @class UBTTask_Wander
 * @brief A Behavior Tree task that makes the AI wander to random locations found via an EQS query.
 */
UCLASS()
class DREADNIGHT_API UBTTask_Wander : public UBTTask_Base
{
	GENERATED_BODY()
	
protected:
	/**
	 * @brief The EQS Query to run to find suitable wander locations.
	 */
	UPROPERTY(EditAnywhere, Category = "Node")
	FValueOrBBKey_Object WanderQuery;

	UPROPERTY(Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	FValueOrBBKey_Float AcceptableRadius;
	
	/**
	 * @brief Blackboard key (bool or object) to set when the wander task should stop.
	 */
	UPROPERTY(EditAnywhere, Category = "Blackboard", DisplayName = "WanderStop")
	FBlackboardKeySelector WanderStopKey;

	/**
	 * @brief If set, the task will invert the condition on the WanderStopKey.
	 */
	UPROPERTY(Category = "Node", EditAnywhere)
	FValueOrBBKey_Bool InvertedCondition;
	
	/**
	 * @brief The time the AI should wait after reaching a wander point before finding a new one.
	 */
	UPROPERTY(Category = "Node", EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	FValueOrBBKey_Float WanderWaitTime;

public:
	UBTTask_Wander(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the task is executed by the Behavior Tree.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Called when the task is aborted.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Called when the underlying Gameplay Task (UAITask_Wander) is deactivated.
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
	
	// Called when the task finishes execution.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	// Initializes blackboard key properties from the Behavior Tree asset.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	// Returns the size of the custom memory struct.
	virtual uint16 GetInstanceMemorySize() const override;
	
	// Checks the blackboard to see if the stop condition has been met.
	bool HasToStop(uint8* NodeMemory);

	EBlackboardNotificationResult OnWanderQueryValueChanged(const UBlackboardComponent& BlackboardComponent,
												   FBlackboard::FKey keyID);

	EBlackboardNotificationResult OnAcceptableRadiusValueChanged(const UBlackboardComponent& BlackboardComponent,
												   FBlackboard::FKey keyID);
	
	EBlackboardNotificationResult OnWanderStopValueChanged(const UBlackboardComponent& BlackboardComponent,
													   FBlackboard::FKey keyID);

	EBlackboardNotificationResult OnInvertedConditionValueChanged(const UBlackboardComponent& BlackboardComponent,
													   FBlackboard::FKey keyID);

	EBlackboardNotificationResult OnWanderWaitTimeValueChanged(const UBlackboardComponent& BlackboardComponent,
													   FBlackboard::FKey keyID);
};