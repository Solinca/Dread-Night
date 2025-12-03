#include "Components/BowCombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"

UBowCombatComponent::UBowCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanShoot = true;
	bIsAiming = false;
}

void UBowCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBowCombatComponent::SetAiming(bool bAiming)
{
	bIsAiming = bAiming;

	// (optionnel) réduire la vitesse du perso en visant :
	// if (auto* Character = Cast<ACharacter>(GetOwner()))
	// {
	//     Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? 200.f : 600.f;
	// }

	// (optionnel) activer un zoom caméra, FOV changes, etc.
}

void UBowCombatComponent::Shoot()
{
	if (!bCanShoot || !ArrowProjectileClass)
		return;

	bCanShoot = false;

	SpawnArrowProjectile();

	// cooldown
	GetWorld()->GetTimerManager().SetTimer(
		ShotCooldownTimer,
		this,
		&UBowCombatComponent::ResetShot,
		ShotCooldown,
		false
	);
}

void UBowCombatComponent::SpawnArrowProjectile()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	//On spawn depuis la caméra ou un socket de la main ?
	FVector SpawnLocation;
	FRotator SpawnRotation;

	//VERSION simple ? tirer depuis la caméra du joueur
	APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);

		SpawnLocation = CamLoc + CamRot.Vector() * 60.f;
		SpawnRotation = CamRot;
	}
	else
	{
		SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;
		SpawnRotation = Owner->GetActorRotation();
	}

	FActorSpawnParameters Params;
	Params.Instigator = Cast<APawn>(Owner);
	Params.Owner = Owner;

	AActor* Arrow = GetWorld()->SpawnActor<AActor>(
		ArrowProjectileClass,
		SpawnLocation,
		SpawnRotation,
		Params
	);

	//si ta flèche a un ProjectileMovementComponent :
	if (Arrow)
	{
		if (UProjectileMovementComponent* MoveComp = Arrow->FindComponentByClass<UProjectileMovementComponent>())
		{
			MoveComp->Velocity = SpawnRotation.Vector() * ArrowSpeed;
		}
	}
}

void UBowCombatComponent::ResetShot()
{
	bCanShoot = true;
}