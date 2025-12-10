// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/ExcludeActor/ExcludeSpline.h"

#include "Components/SplineComponent.h"


AExcludeSpline::AExcludeSpline()
{ 
	PrimaryActorTick.bCanEverTick = false;

	ExcludeSpline = CreateDefaultSubobject<USplineComponent>("Exclude Spline");
	RootComponent = ExcludeSpline;

	ExcludeSpline->SetGenerateOverlapEvents(false);
	ExcludeSpline->SetCollisionResponseToAllChannels(ECR_Ignore);
}
 