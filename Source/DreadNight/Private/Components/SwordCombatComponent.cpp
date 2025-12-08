#include "Components/SwordCombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanAttack = true;
}

void USwordCombatComponent::Attack()
{
	if (!bCanAttack)
		return;
	bCanAttack = false;
	PerformAttackTrace();
	//Set timer to reset attack ability after cooldown
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &USwordCombatComponent::ResetAttack, AttackCooldown, false);
}

void USwordCombatComponent::PerformAttackTrace()
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;
	FVector Start = Owner->GetActorLocation();
	FVector ForwardVector = Owner->GetActorForwardVector();
	FVector End = Start + (ForwardVector * AttackRange);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Owner);
	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		AttackRadius,
		ObjectTypes,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);
	if (bHit)
	{
		for (FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				UGameplayStatics::ApplyDamage(HitActor, AttackDamage, Owner->GetInstigatorController(), Owner, nullptr);
			}
		}
	}
}

void USwordCombatComponent::ResetAttack()
{
	bCanAttack = true;
}