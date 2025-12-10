#include "Components/SwordCombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanAttack = true;
	bIsAttacking = false;
}

void USwordCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!SwordHitBox.IsValid())
		return;
	SwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordHitBox->OnComponentBeginOverlap.AddDynamic(this, &USwordCombatComponent::OnSwordOverlap);
}

void USwordCombatComponent::Attack()
{
	//if (!bCanAttack || !SwordHitBox.IsValid())
	if (!bCanAttack)
		return;
	bCanAttack = false;
	bIsAttacking = true;
	HitActors.Empty();
	//activate collision during the attack
	//EnableSwordCollision();
	GetWorld()->GetTimerManager().SetTimer(CollisionCooldownTimerHandle, this, &USwordCombatComponent::DisableSwordCollision, CollisionEnableTime, false);
	//Set timer to reset attack ability after cooldown
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &USwordCombatComponent::ResetAttack, AttackCooldown, false);
}

void USwordCombatComponent::SetHitBoxComponent(UBoxComponent* HitBox)
{
	SwordHitBox = HitBox;
}

bool USwordCombatComponent::GetIsAttacking()
{
	return bIsAttacking;
}

void USwordCombatComponent::EnableSwordCollision()
{
	if (SwordHitBox.IsValid())
	{
		SwordHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitActors.Empty();
	}
}

void USwordCombatComponent::DisableSwordCollision()
{
	//if (SwordHitBox.IsValid())
	//	SwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsAttacking = false;
}

void USwordCombatComponent::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking || !OtherActor)
		return;
	if (HitActors.Contains(OtherActor)) //avoid touching the same actor multiple times in one attack
		return;
	HitActors.Add(OtherActor);
	UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
}

void USwordCombatComponent::ResetAttack()
{
	bCanAttack = true;
}