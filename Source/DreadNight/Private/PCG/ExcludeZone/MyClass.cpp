// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/ExcludeZone/MyClass.h"

AExcludeZone::AExcludeZone()
{
	PrimaryActorTick.bCanEverTick = false;

	BlockingZone = CreateDefaultSubobject<UBoxComponent>("Exclude Zone");
	RootComponent = BlockingZone;	
}
