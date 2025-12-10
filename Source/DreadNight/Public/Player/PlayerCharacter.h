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
#include "Items/Object/ItemInstance_Weapon.h"
#include "Items/Data/WeaponDataAsset.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

protected:
	APlayerCharacter();
	virtual void BeginPlay() override;

	//===============//
	//  Components   //
	//===============//

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

	//===============//

	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CapsuleMaxHalfHeight = 88.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CurrentCapsuleHalfHeight = CapsuleMaxHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CapsuleCrouchedHalfHeight = 44.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float LerpCrouchSpeed = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UItemDataAsset> StartingWeaponDataAsset = nullptr;

	UItemInstance_Weapon* CurrentIstanceWeapon;

	TObjectPtr<UStaticMeshComponent> CurrentWeaponMesh = nullptr;

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
	void EquipWeapon(UItemInstance_Weapon* itemInstanceWeapon);

	UFUNCTION()
	USwordCombatComponent* GetSwordCombatComponent();
};
