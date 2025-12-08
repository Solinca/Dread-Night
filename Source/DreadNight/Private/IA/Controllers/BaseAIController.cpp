#include "IA/Controllers/BaseAIController.h"

#include "CustomLogCategories.h"
#include "IA/Characters/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Team/FunctionLibraries/TeamFunctionLibrary.h"

ABaseAIController::ABaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");

	UTeamFunctionLibrary::SetTeam(this, Enemy);
}

void ABaseAIController::TryRunBehaviorTree()
{
	TryRunBehaviorTree(GetPawn());
}

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
	if (bAutoRunBehaviorTree)
	{
		//In order to avoid a crash because the super implementation has not been called and GetPawn return nullptr.
		//I created an internal implementation taking a pawn in an argument.
		TryRunBehaviorTree(InPawn);
	}

	//Then OnPossess will call PossessedBy in the character and the blackboard will not be nullptr since the RunBehavior function has been called.
	Super::OnPossess(InPawn);
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

	if (!IsValid(OtherPtr) && OtherPtr->Implements<UGenericTeamAgentInterface>())
	{
		return ETeamAttitude::Neutral;
	}

	// Simple behavior for the team attitude for now but it could change in the future.
	const EGameTeam OtherTeam{UTeamFunctionLibrary::GetTeam(OtherPtr)};
	const EGameTeam ThisTeam{UTeamFunctionLibrary::GetTeam(this)};
	
	return ThisTeam == OtherTeam ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void ABaseAIController::TryRunBehaviorTree(APawn* InPawn)
{
	ABaseAICharacter* BaseAICharacter{Cast<ABaseAICharacter>(InPawn)};
	UMonsterDataAsset* MonsterDataAsset{BaseAICharacter->GetMonsterData()};
	UBehaviorTree* UsedBehaviorTree{MonsterDataAsset->GetBehaviorTree()};
	
	if (!UsedBehaviorTree)
	{
		UE_LOG(LogAIController, Error,
			TEXT("ABaseAIController: Unable to run the behavior because the used behavior tree variable is equal to nullptr."))
		return;
	}
	
	RunBehaviorTree(UsedBehaviorTree);
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
