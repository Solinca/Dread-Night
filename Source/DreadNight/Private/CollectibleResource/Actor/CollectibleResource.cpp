// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleResource/Actor/CollectibleResource.h"

#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"


bool ACollectibleResource::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	//TODO Add cast to player and add inventory logic
	if (DamageInstigator)
	{
		UItemInstance* ItemInstance = FItemInstanceFactory::CreateItem(ItemDataAsset, FMath::Min(Damage, CurrentItemQuantity));
			

		return true;
	}

	if (CurrentItemQuantity <= 0)
	{
		Destroy();
	}

	return false;
}

ACollectibleResource::ACollectibleResource()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	if (RootComponent)
	{
		ResourceMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
 
void ACollectibleResource::BeginPlay()
{
	Super::BeginPlay();

	CurrentItemQuantity = FMath::RandRange(ItemQuantityRange.X,ItemQuantityRange.Y);
}
 

