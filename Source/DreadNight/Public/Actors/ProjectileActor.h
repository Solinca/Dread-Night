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
	
public:

	UProjectileMovementComponent* GetProjectileMovementComponent() const;
	
	AProjectileActor();
};
