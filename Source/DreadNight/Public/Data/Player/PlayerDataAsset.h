#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Widgets/PauseMenu.h"
#include "UI/Widgets/Inventory.h"
#include "UI/Widgets/InventoryBackground.h"
#include "UserWidgets/OptionsWidget.h"
#include "PlayerDataAsset.generated.h"

class UItemNotification;
class UMapWidget;
class UPlayerHud;

UCLASS()
class DREADNIGHT_API UPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	float CapsuleMaxHalfHeight = 88.f;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	float CapsuleCrouchedHalfHeight = 44.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D ViewPitch = FVector2D(-55, 55);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseMoveSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintMoveSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CrouchMoveSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LerpCrouchSpeed = 8.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UItemDataAsset> StartingWeaponDataAsset = nullptr;

	UPROPERTY(EditAnywhere, Category = "Armor")
	TObjectPtr<UItemDataAsset> StartingArmorDataAsset = nullptr;

	UPROPERTY(EditAnywhere, Category = "Armor")
	TObjectPtr<UItemDataAsset> StartingHelmetDataAsset = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HandSocketName = TEXT("WeaponSocket");

	UPROPERTY(EditAnywhere, Category = "Armor")
	FName ArmorSocketName = TEXT("ArmorSocket");

	UPROPERTY(EditAnywhere, Category = "Armor")
	FName HelmetSocketName = TEXT("HelmetSocket");

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName SecondaryHandSocketName = TEXT("BowSocket");

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName ArrowSocketName = TEXT("ArrowSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float JumpStaminaCost = 20.f;

	// TODO: REPLACE WITH WEAPON STAMINA COST LATER
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float AttackStaminaCost = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float SprintStaminaCost = 10.f;

	// TODO: REPLACE WITH WEAPON MANA COST LATER
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
	float SpellManaCost = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Life")
	float PassiveHealthRegenAmount = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Life")
	float PassiveHealthRegenTimer = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float HungerSprintCost = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float HungerJumpCost = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Hunger")
	float HungerAttackCost = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Sanity")
	float SanityOnDamageCost = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sanity")
	float SanityOnDarknessCost = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenu> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UOptionsWidget> OptionsClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventory> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventory> HotbarInventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryBackground> OtherInventoryBackgroundWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerHud> PlayerHudClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMapWidget> MapClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UItemNotification> ItemNotificationClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> GameOverSound;

	UPROPERTY(EditDefaultsOnly, Category = "Levels")
	TSoftObjectPtr<UWorld> MainMenuLevel;
};
