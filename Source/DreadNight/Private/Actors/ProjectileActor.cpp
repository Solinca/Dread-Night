#include "Actors/ProjectileActor.h"

#include "DamageSystem/Interface/Damageable.h"

AProjectileActor::AProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh Component");
	RootComponent = ProjectileMeshComponent;

	ProjectileMeshComponent->SetCollisionProfileName("Projectile");
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Friction = 0.f;
	ProjectileMovementComponent->bSweepCollision = false;

	InitialLifeSpan = 10.f;
}

void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnBeginOverlap);
}

void AProjectileActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || IsPendingKillPending() || !OtherActor->Implements<UDamageable>())
	{
		return;
	}

	IDamageable* Damageable{Cast<IDamageable>(OtherActor)};
	Damageable->TryApplyDamage(Damage, GetInstigator());
	
	Destroy();
}

UProjectileMovementComponent* AProjectileActor::GetProjectileMovementComponent() const
{
	return ProjectileMovementComponent;
}

void AProjectileActor::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

float AProjectileActor::GetDamage() const
{
	return Damage;
}
