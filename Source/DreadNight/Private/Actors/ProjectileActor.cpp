#include "Actors/ProjectileActor.h"

AProjectileActor::AProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh Component");
	RootComponent = ProjectileMeshComponent;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;

	InitialLifeSpan = 10.f;
}

UProjectileMovementComponent* AProjectileActor::GetProjectileMovementComponent() const
{
	return ProjectileMovementComponent;
}