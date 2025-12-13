#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance_Weapon.h"

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

	CurrentWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	HotbarInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("HotbarInventoryComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCapsuleHalfHeight = PlayerData->CapsuleMaxHalfHeight;
	InventoryComponent->SetSize(PlayerData->InventorySize);
	HotbarInventoryComponent->SetSize(PlayerData->HotbarInventorySize);

	EquipWeapon(Cast<UItemInstance_Weapon>(FItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1)));
}

bool APlayerCharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage);

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

	SwordCombatComponent->SetWeapon(CurrentWeaponMesh, Weapon->GetDataAsset()->Damage, PlayerData->WeaponAttackCooldown);
}
