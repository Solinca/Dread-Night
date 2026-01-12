#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerTakeDamageSignature);

UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter, public IDamageable, public ISavableActor
{
	GENERATED_BODY()
	
protected:
	bool bIsDynamicallySpawned = false;
	UClass* DynamicSpawnedClass = nullptr;
	FGuid DynamicUniqueID;
public:
	
	virtual FName GetUniqueIdentifier() const override {return TEXT("Player_One");};
	
	virtual void SetIsDynamicallySpawned(UClass* SpawnClass, FGuid UniqueID = FGuid::NewGuid()) override
	{
		DynamicUniqueID = MoveTemp(UniqueID); DynamicSpawnedClass = SpawnClass; bIsDynamicallySpawned = true;
	};

	virtual bool IsDynamicallySpawned() const override
	{
		return bIsDynamicallySpawned;
	};
	
	virtual UClass* GetSpawnClass() const override
	{
		return DynamicSpawnedClass;
	};
	
	virtual FTransform GetSpawnTransform() const override
	{
		return GetActorTransform();
	};
private:
	bool bIsCrouching = false;

	bool bIsSprinting = false;

	float CurrentCapsuleHalfHeight;

protected:
	APlayerCharacter();

	virtual void BeginPlay() override;

	void TimerHealthRegen();

	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaminaComponent> StaminaComponent = nullptr;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UConditionStateComponent> ConditionStateComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USwordCombatComponent> SwordCombatComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArmorComponent> ArmorComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CurrentItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UInventoryComponent> HotbarInventoryComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBowCombatComponent> BowCombatComponent;

	UPROPERTY(BlueprintReadOnly)
	FName EquippedObjectTag;

	FTimerHandle THHealthRegen;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerTakeDamageSignature OnPlayerTakeDamage;

public:
	UPROPERTY(EditAnywhere, SaveGame)
	int CurrentHotbarIndex = 0;
	
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

	void UnequipCurrentlyHeldItem();

	UFUNCTION()
	void EquipArmor(UItemInstance_Armor* itemInstanceArmor);

	virtual void OnPreSave() override;

	virtual void OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache) override;

	void ProcessHotbarSlot();
};
