#include "IA/Tasks/AITask_Wander.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQuery.h"

UAITask_Wander* UAITask_Wander::Create(AAIController& AIOwner, IGameplayTaskOwnerInterface& InTaskOwner, const FAiTaskWanderParameters& InitParams)
{
	if (UAITask_Wander* NewTask = NewAITask<UAITask_Wander>(AIOwner, InTaskOwner))
	{
		NewTask->AiTaskParameters = InitParams;
		return NewTask;
	}

	return nullptr;
}

FAiTaskWanderParameters& UAITask_Wander::GetAiTaskParameters()
{
	return AiTaskParameters;
}

void UAITask_Wander::OnEQSCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ended query from [AIController : %s] and [AIPawn : %s]"), *GetAIController()->GetName(), *GetAIController()->GetPawn()->GetActorLabel())
	
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		FinishFailedTask();
		return;
	}

	TArray<FVector> QueriedLocations;
	
	UE_CLOG_FAIL_TASK(!QueryInstance->GetQueryResultsAsLocations(QueriedLocations), true, , LogTemp, Error,
	 				  TEXT("UAITask_Wander: The result of the query is empty and does not contain any valid location."))

	CachedQueryRequest = nullptr;

	// Decide whether to move immediately or after a delay.
	if (!FMath::IsNearlyZero(AiTaskParameters.WanderWaitTime))
	{
		MoveToDelayed(QueriedLocations[0], AiTaskParameters.WanderWaitTime);
		return;
	}
	
	MoveTo(QueriedLocations[0]);
}

void UAITask_Wander::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// Ensure this callback is for the move request we initiated.
	if (!CachedMoveRequestID.IsEquivalent(RequestID))
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Ended move request from [AIController : %s] and [AIPawn : %s]"), *GetAIController()->GetName(), *GetAIController()->GetPawn()->GetActorLabel())
	
	UE_CLOG_FAIL_TASK(Result.IsFailure() || Result.IsInterrupted(), Result.IsFailure(), , LogTemp, Error,
		TEXT("UAITask_Wander: The movement request as finished with the result \"Failed\""))

	// Once movement is complete, launch a new query to find the next wander spot.
	UE_CLOG_FAIL_TASK(!LaunchQuery(), true, , LogTemp, Error, TEXT("UAITask_Wander: The query has failed to launch it self."))
}

bool UAITask_Wander::LaunchQuery()
{
	AAIController* AiController{GetAIController()};
	
	UE_CLOG_FAIL_TASK(!AiController, true, false, LogTemp, Error, TEXT("UAITask_Wander: The AI controller is equal to nullptr."))
	
	UE_CLOG_FAIL_TASK(!AiTaskParameters.WanderQuery, true, false, LogTemp, Error,
		TEXT("UBTTask_Wander: The wander query is equal to nullptr."))

	//UE_LOG(LogTemp, Warning, TEXT("Launching query from [AIController : %s] and [AIPawn : %s]"), *AiController->GetName(), *AiController->GetPawn()->GetActorLabel())
	
	UEnvQuery* WanderQuery{AiTaskParameters.WanderQuery};
	
	// Run the EQS query.
	CachedQueryRequest = UEnvQueryManager::RunEQSQuery(
		GetWorld(), WanderQuery, AiController, EEnvQueryRunMode::RandomBest25Pct,
		UEnvQueryInstanceBlueprintWrapper::StaticClass());

	UE_CLOG_FAIL_TASK(!CachedQueryRequest.IsValid(), true, false, LogTemp, Error,
			   TEXT("UAITask_Wander: The query instance is invalid which is due to a failure from the RunEqsQuery "
					"function."))

	// Bind our callback to the query's completion event.
	CachedQueryRequest->GetOnQueryFinishedEvent().AddDynamic(this, &UAITask_Wander::OnEQSCompleted);
	return true;
}

void UAITask_Wander::MoveTo(const FVector& GoalLocation)
{
	AAIController* AiController{GetAIController()};

	UE_CLOG_FAIL_TASK(!AiController, true, , LogTemp, Error, TEXT("UAITask_Wander: The AI controller is equal to nullptr."))

	// Configure and issue a move request.
	FAIMoveRequest MoveRequest{GoalLocation};
	MoveRequest.SetAcceptanceRadius(AiTaskParameters.AcceptableRadius);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetCanStrafe(true);
	MoveRequest.SetRequireNavigableEndLocation(true);
	MoveRequest.SetReachTestIncludesAgentRadius(true);
	MoveRequest.SetReachTestIncludesGoalRadius(true);

	FPathFollowingRequestResult Result{AiController->MoveTo(MoveRequest)};
	
	// Store the move request ID to be able to check it in the callback.
	CachedMoveRequestID = Result.MoveId;
	
	UE_CLOG_FAIL_TASK(Result.Code == EPathFollowingRequestResult::Failed, true, , LogTemp, Error,
					  TEXT("UAITask_Wander: The move request has failed."))

	//UE_LOG(LogTemp, Warning, TEXT("Launching move request from [AIController : %s] and [AIPawn : %s]"), *GetAIController()->GetName(), *GetAIController()->GetPawn()->GetActorLabel())
	//UE_CLOG(Result.Code == EPathFollowingRequestResult::AlreadyAtGoal, LogTemp, Warning, TEXT("Already at goal [AIPawn : %s]."), *GetAIController()->GetPawn()->GetActorLabel());

	if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		// Once movement is complete, launch a new query to find the next wander spot.
		UE_CLOG_FAIL_TASK(!LaunchQuery(), true, , LogTemp, Error, TEXT("UAITask_Wander: The query has failed to launch it self."))
	}
}

void UAITask_Wander::MoveToDelayed(const FVector& GoalLocation, const float Delay)
{
	// Use a timer to delay the movement.
	GetWorld()->GetTimerManager().SetTimer(DelayedTimerHandle, [GoalLocation, this]()
	{
		MoveTo(GoalLocation);
	}, Delay, false);
}

void UAITask_Wander::Activate()
{
	Super::Activate();
	
	// Bind our callback to the path following component's completion event.
	GetAIController()->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UAITask_Wander::OnMoveCompleted);

	if (IsFinished())
	{
		FinishFailedTask();
		return;
	}
	
	// Start the wander loop by launching the first EQS query.
	if (!LaunchQuery())
	{
		FinishFailedTask();
	}
}

void UAITask_Wander::Clean()
{
	if (AAIController* AiController{GetAIController()})
	{
		if (UPathFollowingComponent* PathFollowingComponent = AiController->GetPathFollowingComponent())
        {
            // Unbind the move completion event and abort any active move.
            PathFollowingComponent->OnRequestFinished.RemoveAll(this);
            PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::None, CachedMoveRequestID);
        }
	}

	// Unbind from the EQS query completion event.
	if (CachedQueryRequest.IsValid())
	{
		CachedQueryRequest->GetOnQueryFinishedEvent().RemoveAll(this);
	}
	
	// Ensure any pending delayed move is canceled.
	GetWorld()->GetTimerManager().ClearTimer(DelayedTimerHandle);
}