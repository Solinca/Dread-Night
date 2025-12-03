// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Controllers/BaseAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Team/FunctionLibraries/TeamFunctionLibrary.h"


// Sets default values
ABaseAIController::ABaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");

	UTeamFunctionLibrary::SetTeam(this, Enemy);
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

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionInfoUpdated);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::InternalOnTargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ABaseAIController::OnTargetPerceptionForgotten);
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

void ABaseAIController::BP_OnSightStimulusUpdated_Implementation(AActor* Actor, FAIStimulus& Stimulus, const ETeamAttitude::Type TeamAttitude)
{
	OnSightStimulusUpdated(Actor, Stimulus, TeamAttitude);
}

void ABaseAIController::BP_OnHearingStimulusUpdated_Implementation(AActor* Actor, FAIStimulus& Stimulus,
	const ETeamAttitude::Type TeamAttitude)
{
	OnHearingStimulusUpdated(Actor, Stimulus, TeamAttitude);
}

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	AActor* OtherPtr{const_cast<AActor*>(&Other)};

	if (!IsValid(OtherPtr) && OtherPtr->Implements<IGenericTeamAgentInterface>())
	{
		return ETeamAttitude::Neutral;
	}

	// Simple behavior for team attitude for now but it could change in the future.
	const EGameTeam OtherTeam{UTeamFunctionLibrary::GetTeam(OtherPtr)};
	const EGameTeam ThisTeam{UTeamFunctionLibrary::GetTeam(this)};
	
	return ThisTeam == OtherTeam ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void ABaseAIController::InternalOnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	OnTargetPerceptionUpdated(Actor, Stimulus);

	const ETeamAttitude::Type TeamAttitude{GetTeamAttitudeTowards(*Actor)};

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		BP_OnSightStimulusUpdated(Actor, Stimulus, TeamAttitude);
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		BP_OnHearingStimulusUpdated(Actor, Stimulus, TeamAttitude);
	}
}
