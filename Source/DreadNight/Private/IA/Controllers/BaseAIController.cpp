// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Controllers/BaseAIController.h"


// Sets default values
ABaseAIController::ABaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAIController::TryRunBehaviorTree()
{
	if (!UsedBehaviorTree)
	{
		UE_LOG(LogAIController, Error,
			TEXT("ABaseAIController: Unable to run the behavior because the used behavior tree variable is equal to nullptr."))
		return;
	}

	RunBehaviorTree(UsedBehaviorTree);

	BP_SetupBlackboard(GetBlackboardComponent());
}

// Called when the game starts or when spawned
void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (bAutoRunBehaviorTree)
	{
		TryRunBehaviorTree();
	}
}

void ABaseAIController::BP_SetupBlackboard_Implementation(UBlackboardComponent* BlackboardComponent)
{
	SetupBlackboard(BlackboardComponent);
}

