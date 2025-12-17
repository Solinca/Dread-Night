#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (ClampMin = 0.f))
	float Damage{0.f};
public:	
	AProjectileActor();

	UProjectileMovementComponent* GetProjectileMovementComponent() const;

	void SetDamage(float NewDamage);

	float GetDamage() const;
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
