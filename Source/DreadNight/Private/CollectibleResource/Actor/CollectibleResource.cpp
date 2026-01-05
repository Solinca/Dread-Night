#include "CollectibleResource/Actor/CollectibleResource.h"
#include "CollectibleResource/Datas/CollectibleData.h"
#include "Global/MyGameStateBase.h"
#include "InventorySystem/InventoryComponent.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"
#include "Windows/WindowsApplication.h"
#include "Data/Loot/LootData.h"
#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/RespawnComponent.h"
#include "Player/PlayerCharacter.h"
#include "Subsystems/World/DayCycleSubSystem.h"

bool ACollectibleResource::TryApplyDamage(float Damage, AActor* DamageInstigator)
{	
	if (UInventoryComponent* InventoryComponent = DamageInstigator->GetComponentByClass<UInventoryComponent>())
	{
		DropItem();

		RespawnComponent->Despawn();
	}

	return false;
}

void ACollectibleResource::OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache)
{
	RespawnComponent->OnPostLoad();
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

	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("Respawn Component");
}
 
void ACollectibleResource::BeginPlay()
{
	Super::BeginPlay();

	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)); GameInstance && GameInstance->IsNewGame())
	{
		// TODO : Possibly change this to a random life
		CurrentLife = 1;
	} 
	
	SetMesh();

	SetIsDynamicallySpawned(GetClass());
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

			APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			UInventoryComponent* InventoryComp = PC->GetInventoryComponent();
			UInventoryComponent* HotbarComp = PC->GetHotbarInventoryComponent();
			UItemInstance* ItemInstance = UItemInstanceFactory::CreateItem(InventoryComp->GetOwner(),LootData->ItemDataAsset,RandomStack);
			if (HotbarComp != nullptr && HotbarComp->Contains(LootData->ItemDataAsset, 1) && LootData->ItemDataAsset->StackLimit > 1)
			{
				if (HotbarComp->GetItemAtSlot(HotbarComp->GetItemSlot(LootData->ItemDataAsset).GetValue())->CanBeStackedWith(ItemInstance,UItemInstance::EStackMethod::Fully))
					HotbarComp->AddItem(ItemInstance);
				else
				{
					UItemInstance* ToAddTo = HotbarComp->GetItemAtSlot(HotbarComp->GetItemSlot(LootData->ItemDataAsset).GetValue());
					int amountToAdd = LootData->ItemDataAsset->StackLimit - ToAddTo->GetStackNumber();
					UItemInstance* ItemInstance2 = UItemInstanceFactory::CreateItem(InventoryComp->GetOwner(),LootData->ItemDataAsset,amountToAdd);
					HotbarComp->AddItem(ItemInstance2);
					ItemInstance->TryRemove(amountToAdd);
					if (InventoryComp != nullptr)
					{
						InventoryComp->AddItem(ItemInstance);
					}
				}
				
			}
			else if (InventoryComp != nullptr)
			{
				InventoryComp->AddItem(ItemInstance);
			}
		}
	}
}

void ACollectibleResource::SetMesh()
{
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

