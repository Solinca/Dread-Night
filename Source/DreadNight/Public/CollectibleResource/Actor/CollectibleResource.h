#pragma once

#include "CoreMinimal.h"
#include "DamageSystem/Interface/Damageable.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SavableActor.h"
#include "CollectibleResource.generated.h"

class URespawnComponent;
class UItemDataAsset;

UCLASS()
class DREADNIGHT_API ACollectibleResource : public AActor, public IDamageable, public  ISavableActor
{
public:
	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

	virtual void OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache) override;
private:
	GENERATED_BODY()
	GENERATE_GENERIC_SAVABLE_OBJECT()

public: 
	ACollectibleResource();

protected: 
	virtual void BeginPlay() override;
 
	UPROPERTY(SaveGame)
	int CurrentLife = 1;

	UPROPERTY(SaveGame)
	bool bIsDestroyed;
	
	UPROPERTY(SaveGame)
	int RespawnDayDelay;

	ECollisionEnabled::Type CollisionType;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ResourceMesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> ResourceData;

	UPROPERTY(SaveGame,EditDefaultsOnly)
	TObjectPtr<URespawnComponent> RespawnComponent;
	
	void DropItem() const;

	void SetMesh();

public:

	
};
