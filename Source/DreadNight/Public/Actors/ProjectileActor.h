#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Data/Projectiles/ProjectileDataAsset.h"
#include "ProjectileActor.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Projectile", ClassGroup = "Projectile")
class DREADNIGHT_API AProjectileActor : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Transient)
	TObjectPtr<USoundBase> ArrowImpactSound = nullptr;

protected:
	AProjectileActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UProjectileDataAsset> ProjectileData;

	UPROPERTY(BlueprintReadWrite)
	bool bHasBeenShot = false;

	UPROPERTY()
	float Damage = 0.f;

public:
	UProjectileMovementComponent* GetProjectileMovementComponent() const;

	UStaticMeshComponent* GetMesh();

	void SetDamage(float NewDamage);

	float GetDamage() const;

	bool GetHasBeenShot();

	void SetHasBeenShot(bool Bool);

	void SetImpactSound(USoundBase* ImpactSound);
};
