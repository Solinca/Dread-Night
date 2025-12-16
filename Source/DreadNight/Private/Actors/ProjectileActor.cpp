#include "Actors/ProjectileActor.h"

#include "DamageSystem/Interface/Damageable.h"

AProjectileActor::AProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh Component");
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	ProjectileMeshComponent->SetCollisionProfileName("Projectile");
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	//ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
	//ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
	//ProjectileMovementComponent->ProjectileGravityScale = ProjectileData->GravityScale;
	ProjectileMovementComponent->Friction = 0.f;
	ProjectileMovementComponent->bSweepCollision = false;

	//InitialLifeSpan = ProjectileData->LifeSpan;
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
	Damageable->TryApplyDamage(ProjectileData->Damage, GetInstigator());
	
	Destroy();
}

UProjectileMovementComponent* AProjectileActor::GetProjectileMovementComponent() const
{
	return ProjectileMovementComponent;
}

UStaticMeshComponent* AProjectileActor::GetMesh()
{
	return ProjectileMeshComponent;
}

void AProjectileActor::SetDamage(float NewDamage)
{
	ProjectileData->Damage = NewDamage;
}

float AProjectileActor::GetDamage() const
{
	return ProjectileData->Damage;
}

bool AProjectileActor::GetHasBeenShot()
{
	return bHasBeenShot;
}

void AProjectileActor::SetHasBeenShot(bool Bool)
{
	bHasBeenShot = Bool;
}
