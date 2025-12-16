// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/ExcludeActor/ExcludeBase.h"


void AExcludeBase::SetOrRemoveTag(bool bShouldSet, FString Tag)
{
	if (bShouldSet)
		Tags.AddUnique(*Tag);
	else
		Tags.Remove(*Tag);
}

AExcludeBase::AExcludeBase()
{ 
	PrimaryActorTick.bCanEverTick = false;
}

void AExcludeBase::BeginPlay()
{
	Super::BeginPlay(); 
}

void AExcludeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetOrRemoveTag(bExcludeGrass, "ExcludeGrass");
	SetOrRemoveTag(bExcludeForestCollectible, "ExcludeCollectible");
}
 