#pragma once

#include "CoreMinimal.h"

/**
 * Enumeration representing the different teams in the game.
 */
UENUM(BlueprintType, Blueprintable, Category = "Team")
enum EGameTeam : uint8
{
	/** No team assigned (hidden from selection). */
	None = 0 UMETA(Hidden),

	/** Represents the enemy team. */
	Enemy	 UMETA(DisplayName = "Enemy"),

	/** Represents the player team. */
	Player	 UMETA(DisplayName = "Player"),

	/** Represents any other team. */
	Other	 UMETA(DisplayName = "Other")
};
