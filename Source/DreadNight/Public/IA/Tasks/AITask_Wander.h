#pragma once

#include "CoreMinimal.h"
#include "AITask_Base.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Navigation/PathFollowingComponent.h"
#include "Tasks/AITask.h"
#include "AITask_Wander.generated.h"

/**
 * @struct FAiTaskWanderParameters
 * @brief Structure to hold parameters for the wander task.
 */
struct FAiTaskWanderParameters
{
	float AcceptableRadius{0.f};
	
	// Time to wait at a location before finding a new one.
	float WanderWaitTime{0.f};
	
	// The EQS query to run to find wander locations.
	UEnvQuery* WanderQuery{nullptr};
	
	FAiTaskWanderParameters() = default;
	explicit FAiTaskWanderParameters(float AcceptableRadius, float WanderWaitTime, UEnvQuery* WanderQuery)
		: AcceptableRadius(AcceptableRadius), WanderWaitTime(WanderWaitTime), WanderQuery(WanderQuery)
	{}
};

/**
 * @class UAITask_Wander
 * @brief An AI task that makes an AI agent wander to random locations found via an EQS query.
 */
UCLASS()
class DREADNIGHT_API UAITask_Wander : public UAITask_Base
{
	GENERATED_BODY()

protected:
	FAiTaskWanderParameters AiTaskParameters;
	
	// Stores the ID of the active move request.
	FAIRequestID CachedMoveRequestID;
	// Stores the active EQS query instance.
	TWeakObjectPtr<UEnvQueryInstanceBlueprintWrapper> CachedQueryRequest;

	// Timer handle for delayed movements.
	FTimerHandle DelayedTimerHandle;

public:
	/**
	 * @brief Factory method to create a new instance of UAITask_Wander.
	 * @param AIOwner The AI controller that will own this task.
	 * @param InTaskOwner The gameplay task owner.
	 * @param InitParams The initialization parameters for the task.
	 * @return A new instance of UAITask_Wander, or nullptr on failure.
	 */
	static UAITask_Wander* Create(AAIController& AIOwner, IGameplayTaskOwnerInterface& InTaskOwner, const FAiTaskWanderParameters& InitParams);

	FAiTaskWanderParameters& GetAiTaskParameters();
protected:
	/**
	 * @brief Callback function executed when the EQS query is completed.
	 * @param QueryInstance The instance of the completed query.
	 * @param QueryStatus The final status of the query.
	 */
	UFUNCTION()
	void OnEQSCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	/**
	 * @brief Callback function executed when a move request is completed.
	 * @param RequestID The ID of the completed request.
	 * @param Result The result of the path following.
	 */
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	// Runs the EQS query to find a new wander location.
	bool LaunchQuery();
	
	// Initiates movement to the goal location.
	void MoveTo(const FVector& GoalLocation);
	
	// Initiates movement to the goal location after a delay.
	void MoveToDelayed(const FVector& GoalLocation, const float Delay);

	// Called when the task is started.
	virtual void Activate() override;
	
	// Called to clean up resources when the task ends or is canceled.
	virtual void Clean() override;
};