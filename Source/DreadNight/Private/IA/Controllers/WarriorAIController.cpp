#include "IA/Controllers/WarriorAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void AWarriorAIController::SetupBlackboard(UBlackboardComponent* BlackboardComponent)
{
	//The player character has a chance to not be set at the time SetupBlackboard is set.
	auto RetrievePlayer{[BlackboardComponent, this]
	{
		AActor* PlayerActor{GetWorld()->GetFirstPlayerController()->GetPawn()};
		UE_LOG(LogTemp, Warning, TEXT("%p"), PlayerActor)
		BlackboardComponent->SetValueAsObject("TargetActor", PlayerActor);
	}};

	GetWorldTimerManager().SetTimerForNextTick(RetrievePlayer);
}
