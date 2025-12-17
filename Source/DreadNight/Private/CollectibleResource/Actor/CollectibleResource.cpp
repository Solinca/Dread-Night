#include "CollectibleResource/Actor/CollectibleResource.h"

#include "CollectibleResource/Datas/CollectibleData.h"
#include "Global/MyGameStateBase.h"
#include "InventorySystem/InventoryComponent.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"
#include "Windows/WindowsApplication.h"
#include "Data/Loot/LootData.h"

bool ACollectibleResource::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	const int ResourceCollected = FMath::Min(Damage, CurrentItemQuantity);
	
	//TODO Add cast to player and add inventory logic
	if (UInventoryComponent* InventoryComponent = DamageInstigator->GetComponentByClass<UInventoryComponent>())
	{
		DropItem();
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

void ACollectibleResource::DropItem() const
{
	if (!ResourceData)
	{
		UE_LOG(LogTemp, Error, TEXT("No data table assigned in the class %s"), *GetClass()->GetName());
		return;
	}
	
	float Random = FMath::RandRange(0.0f,100.f);
	TArray<FLootData*> LootDatas;
	ResourceData->GetAllRows("" , LootDatas);
	
	for (const auto& LootData : LootDatas)
	{
		if (Random < LootData->Percentage)
		{
			int RandomStack = FMath::RandRange(LootData->MinDroppedAmount, LootData->MaxDroppedAmount);
			UInventoryComponent* InventoryComp = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UInventoryComponent>();
			if (InventoryComp != nullptr)
				InventoryComp->AddItem(UItemInstanceFactory::CreateItem(InventoryComp->GetOwner(),LootData->ItemDataAsset, RandomStack));
		}
	}
}