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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCapsuleHalfHeight = PlayerData->CapsuleMaxHalfHeight;
	HotbarInventoryComponent->SetSize(GetHotbarInventoryComponent()->GetSize());

	EquipWeapon(Cast<UItemInstance_Weapon>(FItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(FItemInstanceFactory::CreateItem(this, PlayerData->StartingArmorDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(FItemInstanceFactory::CreateItem(this, PlayerData->StartingHelmetDataAsset, 1)));
}

bool APlayerCharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage / ArmorComponent->GetTotalDmgReductionMultiplier());

	return true;
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
	CurrentWeaponMesh->SetStaticMesh(Weapon->GetDataAsset()->WeaponMesh);

	CurrentWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

	SwordCombatComponent->SetWeapon(CurrentWeaponMesh, Weapon->GetDataAsset()->Damage);
}

void APlayerCharacter::EquipArmor(UItemInstance_Armor* Armor)
{
	if (Armor->GetDataAsset()->bIsHelmet)
	{
		CurrentHelmetMesh->SetStaticMesh(Armor->GetDataAsset()->ArmorMesh);

		CurrentHelmetMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

		ArmorComponent->SetHelmetDmgReductionMultiplier(Armor->GetDataAsset()->DamageReductionMultiplier);
	}
	else
	{
		CurrentArmorMesh->SetStaticMesh(Armor->GetDataAsset()->ArmorMesh);

		CurrentArmorMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PlayerData->HandSocketName);

		ArmorComponent->SetArmorDmgReductionMultiplier(Armor->GetDataAsset()->DamageReductionMultiplier);
	}
}

void APlayerCharacter::UnequipArmor()
{
	CurrentArmorMesh->SetStaticMesh(nullptr);

	ArmorComponent->SetArmorDmgReductionMultiplier(0.f);
}

void APlayerCharacter::UnequipHelmet()
{
	CurrentHelmetMesh->SetStaticMesh(nullptr);

	ArmorComponent->SetHelmetDmgReductionMultiplier(0.f);
}
