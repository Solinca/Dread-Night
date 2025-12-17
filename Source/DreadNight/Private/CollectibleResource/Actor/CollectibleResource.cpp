// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleResource/Actor/CollectibleResource.h"

#include "CollectibleResource/Datas/CollectibleData.h"
#include "Global/MyGameStateBase.h"
#include "InventorySystem/InventoryComponent.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"


bool ACollectibleResource::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	const int ResourceCollected = FMath::Min(Damage, CurrentItemQuantity);
	
	//TODO Add cast to player and add inventory logic
	if (UInventoryComponent* InventoryComponent = DamageInstigator->GetComponentByClass<UInventoryComponent>())
	{
		
		UItemInstance* ItemInstance = FItemInstanceFactory::CreateItem(this ,ItemDataAsset, ResourceCollected);

		UE_LOG(LogTemp, Log, TEXT("Resource type = %s, collected %d"), *ItemDataAsset->Type.GetTagLeafName().ToString(), ResourceCollected);

		InventoryComponent->AddItem(ItemInstance);
		
		CurrentItemQuantity -= ResourceCollected;
		if (CurrentItemQuantity <= 0)
		{
			Destroy();
		}
		return true;
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

	AMyGameStateBase* MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	if (!MyGameStateBase)
	{
		return;
	}

	UDataTable* CollectibleMeshDataTable = MyGameStateBase->CollectibleDataTable;
	if (!CollectibleMeshDataTable)
	{
		return;
	}

	TArray<FCollectibleData*> CollectibleDatas;
	CollectibleMeshDataTable->GetAllRows(TEXT("ACollectibleResource::BeginPlay"), CollectibleDatas);

	for (const FCollectibleData* Element : CollectibleDatas)
	{
		if (Element->CollectibleResource != GetClass() || Element->StaticMeshArray.IsEmpty())
		{
			continue;
		}

		const int Random = FMath::RandRange(0, Element->StaticMeshArray.Num() - 1);

		ResourceMesh->SetStaticMesh(Element->StaticMeshArray[Random]);
	}
}
 

