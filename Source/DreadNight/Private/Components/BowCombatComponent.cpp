#include "Components/BowCombatComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UBowCombatComponent::UBowCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanShoot = true;
	bIsAiming = false;
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
	SpawnArrow();

	// cooldown
	GetWorld()->GetTimerManager().SetTimer(
		ShotCooldownTimer,
		this,
		&UBowCombatComponent::ResetShot,
		ShotCooldown,
		false
	);
}

void UBowCombatComponent::SpawnArrow()
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector SpawnLocation;
	FRotator SpawnRotation;
	USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComp)
		return;
	FName HandSocketName = TEXT("hand_rSocket"); //test avec un socket que j'ai ajouté au mannequin unreal, c'est temporaire, faudra ajuster ça avec notre perso
	SpawnLocation = MeshComp->GetSocketLocation(HandSocketName);
	SpawnRotation = MeshComp->GetSocketRotation(HandSocketName);
	FActorSpawnParameters Params;
	AActor* Arrow = GetWorld()->SpawnActor<AActor>(
		ArrowProjectileClass,
		SpawnLocation,
		SpawnRotation,
		Params
	);
}

void UBowCombatComponent::ResetShot()
{
	bCanShoot = true;
}