// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECraftMethod : uint8
{
    Default         UMETA(DisplayName = "Default"),
    Workshop        UMETA(DisplayName = "Workshop"),
    Smeltery        UMETA(DisplayName = "Smeltery"),
    Forge           UMETA(DisplayName = "Forge")
};
