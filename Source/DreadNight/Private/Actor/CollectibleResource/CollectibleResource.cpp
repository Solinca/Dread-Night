// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CollectibleResource/CollectibleResource.h"

 
ACollectibleResource::ACollectibleResource()
{ 
	PrimaryActorTick.bCanEverTick = false;
}
 
void ACollectibleResource::BeginPlay()
{
	Super::BeginPlay();

	CurrentItemQuantity = FMath::RandRange(ItemQuantityRange.X,ItemQuantityRange.Y);
}
 

