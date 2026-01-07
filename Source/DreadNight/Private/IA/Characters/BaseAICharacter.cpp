#include "IA/Characters/BaseAICharacter.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Data/Loot/LootData.h"
#include "InventorySystem/InventoryComponent.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Player/PlayerCharacter.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName("AiPawn");

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddDynamic(this, &ABaseAICharacter::OnDeath);
}

void ABaseAICharacter::OnDeath()
{
	DropLoot();

	Destroy();
}

bool ABaseAICharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage);

	return true;
}

void ABaseAICharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	AIController->SetGenericTeamId(TeamID);
}

FGenericTeamId ABaseAICharacter::GetGenericTeamId() const
{
	return AIController->GetGenericTeamId();
}

ETeamAttitude::Type ABaseAICharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	return AIController->GetTeamAttitudeTowards(Other);
}

void ABaseAICharacter::PossessedBy(AController* NewController)	
{
	Super::PossessedBy(NewController);

	AIController = Cast<AAIController>(NewController);

	UBlackboardComponent* BlackboardComponent{AIController->GetBlackboardComponent()};

	BP_OnDataAssetInitialization(BlackboardComponent, UsedDataAsset);
}

void ABaseAICharacter::SetMonsterData(UMonsterDataAsset* Data)
{
	UsedDataAsset = Data;
}

UMonsterDataAsset* ABaseAICharacter::GetMonsterData() const
{
	return UsedDataAsset;
}

void ABaseAICharacter::DropLoot() const
{
	if (!UsedDataAsset->GetLootDataTable())
	{
		return;
	}

	float Random = FMath::RandRange(0.0f, 100.f);

	TArray<FLootData*> LootDatas;

	UsedDataAsset->GetLootDataTable()->GetAllRows("" , LootDatas);

	for (const auto& LootData : LootDatas)
	{
		if (Random < LootData->Percentage) 
		{
			int RandomStack = FMath::RandRange(LootData->MinDroppedAmount,LootData->MaxDroppedAmount);
			
			APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			UInventoryComponent* InventoryComp = PC->GetInventoryComponent();
			UInventoryComponent* HotbarComp = PC->GetHotbarInventoryComponent();
			UItemInstance* ItemInstance = UItemInstanceFactory::CreateItem(InventoryComp->GetOwner(),LootData->ItemDataAsset,RandomStack);
			if (HotbarComp != nullptr && HotbarComp->Contains(LootData->ItemDataAsset, 1) && LootData->ItemDataAsset->StackLimit > 1)
			{
				if (HotbarComp->GetItemAtSlot(HotbarComp->GetItemSlot(LootData->ItemDataAsset).GetValue())->CanBeStackedWith(ItemInstance,EStackMethod::Fully))
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

void ABaseAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset)
{
	GetMesh()->SetSkeletalMesh(MonsterDataAsset->GetMesh());

	HealthComponent->SetMaxHealth(MonsterDataAsset->GetMaxHealth());
}

void ABaseAICharacter::BP_OnDataAssetInitialization_Implementation(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset)
{
	OnDataAssetInitialization(BlackboardComponent, MonsterDataAsset);
}
