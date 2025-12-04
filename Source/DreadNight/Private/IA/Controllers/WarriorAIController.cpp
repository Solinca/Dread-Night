// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Controllers/WarriorAIController.h"

#include "CustomLogCategories.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"

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
