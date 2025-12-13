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
#include "DamageSystem/Interface/Damageable.h"
#include "Components/SwordCombatComponent.h"
#include "Items/Data/WeaponDataAsset.h"
#include "Data/Player/PlayerDataAsset.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

private:
	bool bIsCrouching = false;

	bool bIsSprinting = false;

	float CurrentCapsuleHalfHeight;

	TObjectPtr<UStaticMeshComponent> CurrentWeaponMesh = nullptr;

protected:
	APlayerCharacter();

	virtual void BeginPlay() override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

public:
	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

	UFUNCTION()
	bool GetIsCrouching();

	UFUNCTION()
	void SetIsCrouching(bool value);

	UFUNCTION()
	bool GetIsSprinting();

	UFUNCTION()
	void SetIsSprinting(bool value);

	UFUNCTION()
	float GetCurentCapsuleHalfHeight();

	UFUNCTION()
	void SetCurentCapsuleHalfHeight(float value);

	UFUNCTION()
	void UpdateCrouching(float deltatime);

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
	void EquipWeapon(UItemInstance_Weapon* itemInstanceWeapon);
};
