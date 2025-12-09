#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Team/TeamType.h"
#include "TeamFunctionLibrary.generated.h"

class AAIController;

/**
 * A library of utility functions for managing teams in the game.
 */
UCLASS(Category = "Team|FunctionLibrary", ClassGroup = "Team")
class DREADNIGHT_API UTeamFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Sets the team for a given object implementing the IGenericTeamAgentInterface.
	 * 
	 * @param TeamAgentInterface - The object implementing the IGenericTeamAgentInterface.
	 * @param GameTeam - The team to assign to the object.
	 */
	static void SetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface, const EGameTeam GameTeam);

	/**
	 * Sets the team for a given AIController.
	 * 
	 * @param AIController - The AIController whose team is to be set.
	 * @param GameTeam - The team to assign to the AIController.
	 */
	static void SetTeam(AAIController* AIController, const EGameTeam GameTeam);

	/**
	 * Retrieves the team of a given object implementing the IGenericTeamAgentInterface.
	 *
	 * @param TeamAgentInterface - The object implementing the IGenericTeamAgentInterface.
	 * @return The team assigned to the object.
	 */
	static EGameTeam GetTeam(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface);

	/**
	 * Retrieves the team of a given AIController.
	 *
	 * @param AIController - The AIController whose team is to be retrieved.
	 * @return The team assigned to the AIController.
	 */
	static EGameTeam GetTeam(const AAIController* AIController);

	/**
	 * Checks if the team of a given object implementing the IGenericTeamAgentInterface matches the specified team.
	 *
	 * @param TeamAgentInterface - The object implementing the IGenericTeamAgentInterface.
	 * @param GameTeam - The team to compare against.
	 * @return True if the object's team matches the specified team, false otherwise.
	 */
	static bool IsEqual(const TScriptInterface<IGenericTeamAgentInterface>& TeamAgentInterface, const EGameTeam GameTeam);

	/**
	 * Checks if the team of a given AIController matches the specified team.
	 *
	 * @param AIController - The AIController whose team is to be compared.
	 * @param GameTeam - The team to compare against.
	 * @return True if the AIController's team matches the specified team, false otherwise.
	 */
	static bool IsEqual(const AAIController* AIController, const EGameTeam GameTeam);
};
