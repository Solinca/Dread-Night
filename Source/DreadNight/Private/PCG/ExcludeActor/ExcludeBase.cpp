// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/ExcludeActor/ExcludeBase.h"

 
AExcludeBase::AExcludeBase()
{ 
	PrimaryActorTick.bCanEverTick = false;
}

void AExcludeBase::BeginPlay()
{
	Super::BeginPlay();

	if (bAffectGrassToo)
		Tags.Add("ExcludeGrass");
}

void AExcludeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bAffectGrassToo)
		Tags.AddUnique("ExcludeGrass");
	else
		Tags.Remove("ExcludeGrass");

}
 