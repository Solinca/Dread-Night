// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExcludeBase.h"
#include "GameFramework/Actor.h"
#include "ExcludeSpline.generated.h"

class USplineComponent;

UCLASS()
class DREADNIGHT_API AExcludeSpline : public AExcludeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> ExcludeSpline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RoadWidth;
	
	AExcludeSpline();
 
 
};
