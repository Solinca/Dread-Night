// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Team/TeamType.h"
#include "TeamFunctionLibrary.generated.h"

class AAIController;

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UTeamFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface, const EGameTeam GameTeam);
	static void SetTeam(AAIController* AIController, const EGameTeam GameTeam);

	static EGameTeam GetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface);
	static EGameTeam GetTeam(const AAIController* AIController);
	
	static bool IsEqual(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface, const EGameTeam GameTeam);
	static bool IsEqual(const AAIController* AIController, const EGameTeam GameTeam);
};
