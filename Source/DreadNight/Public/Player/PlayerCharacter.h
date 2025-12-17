#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/ManaComponent.h"
#include "Components/ConditionStateComponent.h"
#include "Components/ArmorComponent.h"
#include "DamageSystem/Interface/Damageable.h"
#include "Components/SwordCombatComponent.h"
#include "Items/Data/WeaponDataAsset.h"
#include "Items/Data/ArmorDataAsset.h"
#include "Data/Player/PlayerDataAsset.h"
#include "Components/BowCombatComponent.h"
#include "SaveSystem/SavableActor.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter, public IDamageable, public ISavableActor
{
	GENERATED_BODY()
	GENERATE_GENERIC_SAVABLE_OBJECT()
private:
	bool bIsCrouching = false;

	bool bIsSprinting = false;

	float CurrentCapsuleHalfHeight;

protected:
	APlayerCharacter();

	virtual void BeginPlay() override;
	void TimerHealthRegen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaminaComponent> StaminaComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UManaComponent> ManaComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UConditionStateComponent> ConditionStateComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USwordCombatComponent> SwordCombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArmorComponent> ArmorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CurrentWeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CurrentArmorMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CurrentHelmetMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Components")
	TObjectPtr<UInventoryComponent> HotbarInventoryComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBowCombatComponent> BowCombatComponent;

	UPROPERTY(BlueprintReadOnly)
	FName EquippedObjectTag;

	FTimerHandle THHealthRegen;

public:
	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

	UFUNCTION()
	UPlayerDataAsset* GetData();

	UFUNCTION()
	bool GetIsCrouching();

	UFUNCTION()
	void SetIsCrouching(bool value);

	UFUNCTION()
	bool GetIsSprinting();

	UFUNCTION(BlueprintCallable)
	bool GetCanShoot();

	UFUNCTION()
	void SetIsSprinting(bool value);

	UFUNCTION()
	float GetCurentCapsuleHalfHeight();

	UFUNCTION()
	void SetCurentCapsuleHalfHeight(float value);

	UFUNCTION()
	void UpdateCrouching(float deltatime);

	UFUNCTION()
	UCameraComponent* GetCamera();

	UFUNCTION()
	UStaminaComponent* GetStaminaComponent();

	UFUNCTION()
	UManaComponent* GetManaComponent();

	UFUNCTION()
	UHealthComponent* GetHealthComponent();

	UFUNCTION()
	UConditionStateComponent* GetConditionStateComponent();

	UFUNCTION()
	USwordCombatComponent* GetSwordCombatComponent();

	UFUNCTION()
	UArmorComponent* GetArmorComponent();

	UFUNCTION()
	UInventoryComponent* GetInventoryComponent();
	
	UFUNCTION()
	UInventoryComponent* GetHotbarInventoryComponent();
	
	UFUNCTION()
	UBowCombatComponent* GetBowCombatComponent();
	
	UFUNCTION()
	void EquipWeapon(UItemInstance_Weapon* itemInstanceWeapon);

	UFUNCTION()
	void EquipArmor(UItemInstance_Armor* itemInstanceArmor);

	UFUNCTION()
	void UnequipArmor();

	UFUNCTION()
	void UnequipHelmet();

	UFUNCTION()
	void SetupArmorComponent();

	UFUNCTION()
	void SetupSwordComponent();

	UFUNCTION()
	void SetupBowComponent();

	UFUNCTION()
	FName GetEquippedObjectTag();

	UFUNCTION()
	void SetEquippedObjectTag(FName NewTag);

	virtual void OnPreSave() override;

	virtual void OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache) override;
};
