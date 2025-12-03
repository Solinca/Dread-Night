// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, Blueprintable)
enum EGameTeam : uint8
{
	None = 0 UMETA(Hidden),
	Enemy	 UMETA(DisplayName = "Enemy"),
	Player	 UMETA(DisplayName = "Player"),
	Other	 UMETA(DisplayName = "Other")
};
