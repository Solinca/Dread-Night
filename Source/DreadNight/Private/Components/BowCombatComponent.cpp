#include "Components/BowCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Player/PlayerCharacter.h"

UBowCombatComponent::UBowCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBowCombatComponent::SetAiming(bool bAiming)
{
	bIsAiming = bAiming;

	if (CurrentArrow == nullptr && bIsAiming)
	{
		SpawnArrow();

		return;
	}

	if (CurrentArrow.IsValid() && !bIsAiming)
	{
		CurrentArrow->Destroy();

		CurrentArrow = nullptr;
	}
}

void UBowCombatComponent::Shoot()
{
	if (!bCanShoot || !ArrowProjectileClass || !CurrentArrow.IsValid())
	{
		return;
	}

	UProjectileMovementComponent* ProjectileComp = CurrentArrow->GetProjectileMovementComponent();
	
	if (ProjectileComp)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

		FVector Direction = (Player->GetCamera()->GetComponentLocation() + Player->GetCamera()->GetForwardVector() * 1000.f) - CurrentArrow->GetActorLocation();
		
		Direction.Normalize();
		
		ProjectileComp->Velocity = Direction * ProjectileComp->InitialSpeed;
		
		ProjectileComp->Activate();
		
		CurrentArrow->SetDamage(ArrowData->Damage);
		
		CurrentArrow->SetHasBeenShot(true);
		
		CurrentArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		CurrentArrow->GetMesh()->SetCollisionProfileName("Arrow");

		UGameplayStatics::PlaySound2D(GetWorld(), PlayerData->PlayerShootArrowSound);

		CurrentArrow->SetImpactSound(PlayerData->PlayerArrowImpactSound);
	}

	CurrentArrow = nullptr;

	bCanShoot = false;

	GetWorld()->GetTimerManager().SetTimer(ShotCooldownTimer, this, &UBowCombatComponent::ResetShot, ShotCooldown, false);
}

bool UBowCombatComponent::CanShoot()
{
	return bCanShoot;
}

bool UBowCombatComponent::IsAiming()
{
	return bIsAiming;
}

void UBowCombatComponent::SpawnArrow()
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		return;
	}

	FVector SpawnLocation;

	FRotator SpawnRotation;

	USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
	
	if (!MeshComp)
	{
		return;
	}

	SpawnLocation = MeshComp->GetSocketLocation(PlayerData->ArrowSocketName);
	
	SpawnRotation = Owner->GetActorRotation();
	
	FActorSpawnParameters Params;
	
	Params.Instigator = Cast<APawn>(Owner);
	
	CurrentArrow = GetWorld()->SpawnActor<AProjectileActor>(ArrowProjectileClass, SpawnLocation, SpawnRotation, Params);

	if (!CurrentArrow.IsValid())
	{
		return;
	}

	CurrentArrow->GetProjectileMovementComponent()->Deactivate();
	
	CurrentArrow->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, PlayerData->ArrowSocketName);
}

void UBowCombatComponent::ResetShot()
{
	bCanShoot = true;
}
