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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UProjectileDataAsset> ProjectileData;

	UPROPERTY(BlueprintReadWrite)
	bool bHasBeenShot = false;
public:	
	AProjectileActor();

	UProjectileMovementComponent* GetProjectileMovementComponent() const;

	UStaticMeshComponent* GetMesh();

	void SetDamage(float NewDamage);

	float GetDamage() const;

	bool GetHasBeenShot();

	void SetHasBeenShot(bool Bool);
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
