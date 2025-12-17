// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECraftMethod : uint8
{
    Basic           UMETA(DisplayName = "Basic"),
    Workshop        UMETA(DisplayName = "Workshop"),
    Smeltery        UMETA(DisplayName = "Smeltery"),
    Forge           UMETA(DisplayName = "Forge")
};