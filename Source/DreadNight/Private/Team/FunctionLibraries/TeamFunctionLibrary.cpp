// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/FunctionLibraries/TeamFunctionLibrary.h"

#include "AIController.h"

void UTeamFunctionLibrary::SetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface,
                                   const EGameTeam GameTeam)
{
	IGenericTeamAgentInterface* GenericTeamAgentInterface{TeamAgentInterface.GetInterface()};
	GenericTeamAgentInterface->SetGenericTeamId(static_cast<uint8>(GameTeam));
}

void UTeamFunctionLibrary::SetTeam(AAIController* AIController, const EGameTeam GameTeam)
{
	AIController->SetGenericTeamId(static_cast<uint8>(GameTeam));
}

EGameTeam UTeamFunctionLibrary::GetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface)
{
	IGenericTeamAgentInterface* GenericTeamAgentInterface{TeamAgentInterface.GetInterface()};
	return static_cast<EGameTeam>(GenericTeamAgentInterface->GetGenericTeamId().GetId());
}

EGameTeam UTeamFunctionLibrary::GetTeam(const AAIController* AIController)
{
	return static_cast<EGameTeam>(AIController->GetGenericTeamId().GetId());
}

bool UTeamFunctionLibrary::IsEqual(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface,
                                   const EGameTeam GameTeam)
{
	IGenericTeamAgentInterface* GenericTeamAgentInterface{TeamAgentInterface.GetInterface()};
	return GenericTeamAgentInterface->GetGenericTeamId().GetId() == static_cast<uint8>(GameTeam);
}

bool UTeamFunctionLibrary::IsEqual(const AAIController* AIController, const EGameTeam GameTeam)
{
	return AIController->GetGenericTeamId().GetId() == static_cast<uint8>(GameTeam);
}
