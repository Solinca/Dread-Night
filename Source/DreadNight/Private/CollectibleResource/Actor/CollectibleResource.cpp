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
#include "Subsystems/World/DayCycleSubSystem.h"

bool ACollectibleResource::TryApplyDamage(float Damage, AActor* DamageInstigator)
{	
	if (UInventoryComponent* InventoryComponent = DamageInstigator->GetComponentByClass<UInventoryComponent>())
	{
		DropItem();
		--CurrentLife;
		if (CurrentLife <= 0)
		{
			RespawnDayDelay = 1;
			TemporaryDestroyCollectible();
		}
		return true;
	}
	

	return false;
}

void ACollectibleResource::OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache)
{
	++RespawnDayDelay;
	if (!bIsDestroyed)
	{
		RespawnCollectible();
	}
	else
	{
		TemporaryDestroyCollectible();
	}
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

	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));
		GameInstance && GameInstance->IsNewGame())
	{
		CurrentLife = 1;//TODO : Possibly change this to a random life
	} 
	
	SetMesh();
	SetIsDynamicallySpawned(GetClass());

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->OnDawnStart.AddDynamic(this, &ACollectibleResource::HealCollectible);
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

void ACollectibleResource::HealCollectible()
{
	--RespawnDayDelay;
	if (RespawnDayDelay <= 0)
	{
		RespawnCollectible();
	}	
}

void ACollectibleResource::TemporaryDestroyCollectible()
{
	bIsDestroyed = true;
	CollisionType = ResourceMesh->GetCollisionEnabled();
	SetActorEnableCollision(false);  
	ResourceMesh->SetVisibility(false, true);
}

void ACollectibleResource::RespawnCollectible()
{
	bIsDestroyed = false;
	CurrentLife = 1;
 
	SetActorEnableCollision(true);  
	ResourceMesh->SetVisibility(true, false);
}
