// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageSystem/Interface/Damageable.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SavableActor.h"
#include "CollectibleResource.generated.h"

class UItemDataAsset;

UCLASS()
class DREADNIGHT_API ACollectibleResource : public AActor, public IDamageable, public  ISavableActor
{
public:
	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

private:
	GENERATED_BODY()
	GENERATE_GENERIC_SAVABLE_OBJECT()

public: 
	ACollectibleResource();

protected: 
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	const TObjectPtr<UItemDataAsset> ItemDataAsset;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ItemQuantityRange;

	UPROPERTY(SaveGame)
	int CurrentItemQuantity;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ResourceMesh;
	
public:

	
};
