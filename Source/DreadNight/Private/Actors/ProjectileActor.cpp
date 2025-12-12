#include "Actors/ProjectileActor.h"

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
	if (OtherActor == this || IsPendingKillPending())
	{
		return;
	}

	const FString DebugMessage{FString::Printf(TEXT("Projectile shoot by %s hit %s."), *Owner->GetName(), *OtherActor->GetName())};
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, DebugMessage, true);

	Destroy();
}

UProjectileMovementComponent* AProjectileActor::GetProjectileMovementComponent() const
{
	return ProjectileMovementComponent;
}