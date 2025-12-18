#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance_Weapon.h"
#include "Items/Object/ItemInstance_Armor.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");

	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	Camera->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina");

	ManaComponent = CreateDefaultSubobject<UManaComponent>("Mana");

	ConditionStateComponent = CreateDefaultSubobject<UConditionStateComponent>("ConditionState");

	SwordCombatComponent = CreateDefaultSubobject<USwordCombatComponent>("SwordCombatComponent");
	
	ArmorComponent = CreateDefaultSubobject<UArmorComponent>("ArmorComponent");

	CurrentWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");

	CurrentArmorMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmorMesh");

	CurrentHelmetMesh = CreateDefaultSubobject<UStaticMeshComponent>("HelmetMesh");
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	HotbarInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("HotbarInventoryComponent");

	BowCombatComponent = CreateDefaultSubobject<UBowCombatComponent>("BowCombatComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCapsuleHalfHeight = PlayerData->CapsuleMaxHalfHeight;
	HotbarInventoryComponent->SetSize(GetHotbarInventoryComponent()->GetSize());
	SetupSwordComponent();
	SetupBowComponent();
	SetupArmorComponent();

	if (PlayerData->StartingWeaponDataAsset)
		HotbarInventoryComponent->AddItem(UItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1));
	EquipWeapon(Cast<UItemInstance_Weapon>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingArmorDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingHelmetDataAsset, 1)));
}

void APlayerCharacter::TimerHealthRegen()
{
	HealthComponent->AddHealth(PlayerData->PassiveHealthRegenAmount * GetConditionStateComponent()->GetHungerValueRatio());

	if (HealthComponent->GetHealthRatio() == 1) GetWorldTimerManager().ClearTimer(THHealthRegen);
}

bool APlayerCharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage / ArmorComponent->GetTotalDmgReductionMultiplier());

	if (HealthComponent->GetHealthRatio() < 1)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(THHealthRegen))
		{
			GetWorld()->GetTimerManager().SetTimer(THHealthRegen, this, &APlayerCharacter::TimerHealthRegen, PlayerData->PassiveHealthRegenTimer, true);
		}
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

UManaComponent* APlayerCharacter::GetManaComponent()
{
	return ManaComponent;
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
 
void APlayerCharacter::EquipWeapon(UItemInstance_Weapon* Weapon)
{
	if (Weapon != nullptr)
	{
		if (Weapon->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Sword"))
			SwordCombatComponent->SetWeapon(Weapon->GetDataAsset());
		else if (Weapon->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Bow"))
			BowCombatComponent->SetWeapon(Weapon->GetDataAsset());
	}
}

void APlayerCharacter::EquipArmor(UItemInstance_Armor* Armor)
{
	if (Armor != nullptr)
	{
		Armor->Use(this);
	}
}

void APlayerCharacter::UnequipArmor()
{ 
	ArmorComponent->EquipArmor(nullptr);
}

void APlayerCharacter::UnequipHelmet()
{
	ArmorComponent->EquipHelmet(nullptr);
}

void APlayerCharacter::SetupArmorComponent()
{  
	CurrentHelmetMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);
	CurrentArmorMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

	ArmorComponent->SetupMesh(CurrentHelmetMesh,CurrentArmorMesh);
}

void APlayerCharacter::SetupSwordComponent()
{
	CurrentWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

	SwordCombatComponent->SetComponentMesh(CurrentWeaponMesh);
}

void APlayerCharacter::SetupBowComponent()
{
	CurrentWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->SecondaryHandSocketName);

	BowCombatComponent->SetComponentMesh(CurrentWeaponMesh);
}

FName APlayerCharacter::GetEquippedObjectTag()
{
	return EquippedObjectTag;
}

void APlayerCharacter::SetEquippedObjectTag(FName NewTag)
{
	EquippedObjectTag = NewTag;
}

void APlayerCharacter::OnPreSave()
{
	InventoryComponent->SerializeInventory();
}

void APlayerCharacter::OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache)
{
	InventoryComponent->DeserializeInventory();
}

UBowCombatComponent* APlayerCharacter::GetBowCombatComponent()
{
	return BowCombatComponent;
}
