#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global/MyGameInstance.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance_Weapon.h"
#include "Items/Object/ItemInstance_Armor.h"
#include "Items/Object/ItemInstance_Building.h"
#include "Player/CustomPlayerController.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");

	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	Camera->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina");

	ConditionStateComponent = CreateDefaultSubobject<UConditionStateComponent>("ConditionState");

	SwordCombatComponent = CreateDefaultSubobject<USwordCombatComponent>("SwordCombatComponent");
	
	ArmorComponent = CreateDefaultSubobject<UArmorComponent>("ArmorComponent");

	CurrentItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	HotbarInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("HotbarInventoryComponent");

	BowCombatComponent = CreateDefaultSubobject<UBowCombatComponent>("BowCombatComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCapsuleHalfHeight = PlayerData->CapsuleMaxHalfHeight;
	
	HotbarInventoryComponent->SetSize(GetHotbarInventoryComponent()->GetSize());

	UMyGameInstance* GameInstance = GetGameInstance<UMyGameInstance>();
	
	if (PlayerData->StartingWeaponDataAsset && GameInstance->IsNewGame())
	{
		HotbarInventoryComponent->AddItem(UItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1));

		ProcessHotbarSlot();
	}
}

void APlayerCharacter::TimerHealthRegen()
{
	HealthComponent->AddHealth(PlayerData->PassiveHealthRegenAmount * GetConditionStateComponent()->GetHungerValueRatio());

	if (HealthComponent->GetHealthRatio() == 1)
	{
		GetWorldTimerManager().ClearTimer(THHealthRegen);
	}
}

bool APlayerCharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage - ArmorComponent->GetTotalDamageReduction());

	OnPlayerTakeDamage.Broadcast();

	UGameplayStatics::PlaySound2D(this, PlayerData->PlayerTakesDamageSound);

	if (HealthComponent->GetHealthRatio() < 1 && !GetWorld()->GetTimerManager().IsTimerActive(THHealthRegen))
	{
		GetWorld()->GetTimerManager().SetTimer(THHealthRegen, this, &APlayerCharacter::TimerHealthRegen, PlayerData->PassiveHealthRegenTimer, true);
	}

	return true;
}

UPlayerDataAsset* APlayerCharacter::GetData()
{
	return PlayerData;
}

bool APlayerCharacter::GetIsCrouching()
{
	return bIsCrouching;
}

void APlayerCharacter::SetIsCrouching(bool value)
{
	bIsCrouching = value;
}

bool APlayerCharacter::GetIsSprinting()
{
	return bIsSprinting;
}

bool APlayerCharacter::GetCanShoot()
{
	return BowCombatComponent->CanShoot();
}

void APlayerCharacter::SetIsSprinting(bool value)
{
	bIsSprinting = value;
}

float APlayerCharacter::GetCurentCapsuleHalfHeight()
{
	return CurrentCapsuleHalfHeight;
}

void APlayerCharacter::SetCurentCapsuleHalfHeight(float value)
{
	CurrentCapsuleHalfHeight = value;

	GetCapsuleComponent()->SetCapsuleHalfHeight(value);
}

void APlayerCharacter::UpdateCrouching(float deltatime)
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		if (bIsCrouching)
		{
			SetCurentCapsuleHalfHeight(FMath::FInterpTo(CurrentCapsuleHalfHeight, PlayerData->CapsuleCrouchedHalfHeight, deltatime, PlayerData->LerpCrouchSpeed));
		}
		else
		{
			SetCurentCapsuleHalfHeight(FMath::FInterpTo(CurrentCapsuleHalfHeight, PlayerData->CapsuleMaxHalfHeight, deltatime, PlayerData->LerpCrouchSpeed));
		}
	}
}

UCameraComponent* APlayerCharacter::GetCamera()
{
	return Camera;
}

UStaminaComponent* APlayerCharacter::GetStaminaComponent()
{
	return StaminaComponent;
}

UHealthComponent* APlayerCharacter::GetHealthComponent()
{
	return HealthComponent;
}

UConditionStateComponent* APlayerCharacter::GetConditionStateComponent()
{
	return ConditionStateComponent;
}

USwordCombatComponent* APlayerCharacter::GetSwordCombatComponent()
{
	return SwordCombatComponent;
}

UArmorComponent* APlayerCharacter::GetArmorComponent()
{
	return ArmorComponent;
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent()
{
	return InventoryComponent;
}

UInventoryComponent* APlayerCharacter::GetHotbarInventoryComponent()
{
	return HotbarInventoryComponent;
}

void APlayerCharacter::UnequipCurrentlyHeldItem()
{
	CurrentItemMesh->OnComponentBeginOverlap.RemoveDynamic(this, &APlayerCharacter::OnSwordOverlap);
	
	CurrentItemMesh->SetStaticMesh(nullptr);
	
	SwordCombatComponent->SetWeapon(nullptr);
}

void APlayerCharacter::EquipArmor(UItemInstance_Armor* Armor)
{
	if (Armor != nullptr)
	{
		Armor->Use(this);
	}
}

void APlayerCharacter::OnPreSave()
{
	InventoryComponent->SerializeInventory();
	
	HotbarInventoryComponent->SerializeInventory();
	 
}

void APlayerCharacter::OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache)
{
	InventoryComponent->DeserializeInventory();
	
	HotbarInventoryComponent->DeserializeInventory();
	 
	
	ProcessHotbarSlot();
	
	StaminaComponent->RegenStamina(0.f);

	TryApplyDamage(0.f, this); //Force the regen timer to start

	if (UMyGameInstance* GameInstance = GetWorld()->GetGameInstance<UMyGameInstance>())
	{
		GameInstance->OnPCGEndGeneration.AddDynamic(ArmorComponent, &UArmorComponent::UpdateAllUI);
		GameInstance->OnPCGEndGeneration.AddDynamic(this, &APlayerCharacter::OnPostLoadUI);
	}
	
}

void APlayerCharacter::ProcessHotbarSlot()
{ 
	UItemInstance* Item = GetHotbarInventoryComponent()->GetItemAtSlot(CurrentHotbarIndex);

	if (!Item)
	{
		return;
	}

	if (UItemInstance_Building* BuildingItem = Cast<UItemInstance_Building>(Item); BuildingItem)
	{
		BuildingItem->Use(this);

		return;
	}

	CurrentItemMesh->SetStaticMesh(Item->GetDataAsset()->ItemMesh);

	CurrentItemMesh->SetRelativeScale3D(Item->GetDataAsset()->ItemScale);
	
	CurrentItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

	if (Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Sword"))
	{
		SwordCombatComponent->SetWeapon(Cast<UItemInstance_Weapon>(Item)->GetDataAsset());

		CurrentItemMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSwordOverlap);
	}
	else if (Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Bow"))
	{
		CurrentItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->SecondaryHandSocketName);
	}
}

void APlayerCharacter::OnPostLoadUI()
{
	GetHotbarInventoryComponent()->OnSelectedHotbarChanged.Broadcast(CurrentHotbarIndex);
	GetHotbarInventoryComponent()->UpdateAllUI();
}


void APlayerCharacter::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwordCombatComponent->OnSwordOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

UBowCombatComponent* APlayerCharacter::GetBowCombatComponent()
{
	return BowCombatComponent;
}
