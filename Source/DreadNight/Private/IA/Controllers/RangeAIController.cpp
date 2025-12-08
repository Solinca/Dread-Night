#include "IA/Controllers/RangeAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void ARangeAIController::SetupBlackboard(UBlackboardComponent* BlackboardComponent)
{
	//The player character has a chance to not be set at the time SetupBlackboard is set.
	auto RetrievePlayer{[BlackboardComponent, this]
	{
		AActor* PlayerActor{GetWorld()->GetFirstPlayerController()->GetPawn()};
		BlackboardComponent->SetValueAsObject("TargetActor", PlayerActor);
	}};

	GetWorldTimerManager().SetTimerForNextTick(RetrievePlayer);
}

