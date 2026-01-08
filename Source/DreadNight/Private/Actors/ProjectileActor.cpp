#include "Actors/ProjectileActor.h"
#include "IA/Characters/BaseAICharacter.h"
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

	ProjectileMovementComponent->Friction = 0.f;
	ProjectileMovementComponent->bSweepCollision = false;
}

void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileData)
	{
		SetDamage(ProjectileData->Damage);
	}

	SetLifeSpan(5);

	ProjectileMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnBeginOverlap);
}

void AProjectileActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || IsPendingKillPending() || !OtherActor->Implements<UDamageable>())
	{
		ProjectileMovementComponent->Deactivate();

		return;
	}

	if (ProjectileMovementComponent->IsActive())
	{
		AttachToComponent(OtherComponent, FAttachmentTransformRules::KeepWorldTransform);

		ProjectileMovementComponent->Deactivate();

		if (TObjectPtr<ACharacter> Character = Cast<ACharacter>(OtherActor))
		{
			IDamageable* Damageable{ Cast<IDamageable>(OtherActor) };
			
			Damageable->TryApplyDamage(Damage, GetInstigator());
		}
	}
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
	Damage = NewDamage;
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
