// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/ExcludeActor/ExcludeZone.h"

AExcludeZone::AExcludeZone()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingZone = CreateDefaultSubobject<UBoxComponent>("Exclude Zone");
	RootComponent = BlockingZone;

	BlockingZone->SetGenerateOverlapEvents(false);
	BlockingZone->SetCollisionResponseToAllChannels(ECR_Ignore);	
}
