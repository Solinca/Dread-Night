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
	SetupSwordComponent();
	SetupArmorComponent();

	EquipWeapon(Cast<UItemInstance_Weapon>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingWeaponDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingArmorDataAsset, 1)));
	EquipArmor(Cast<UItemInstance_Armor>(UItemInstanceFactory::CreateItem(this, PlayerData->StartingHelmetDataAsset, 1)));
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

    SwordCombatComponent->SetWeapon(Weapon->GetDataAsset());
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
