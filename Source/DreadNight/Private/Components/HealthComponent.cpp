#include "Components/HealthComponent.h"
#include "Player/PlayerCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(float Damage, AActor* DamageInstigator)
{
	TryApplyDamage(Damage, DamageInstigator);

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	//If the damage dealer is not the player. Then the player is the one to take damage
	if (!Cast<APlayerCharacter>(DamageInstigator))
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
		OnDamageTaken.Broadcast(CurrentHealth, MaxHealth);
	}
}

void UHealthComponent::AddMaxHealth(float amount)
{
	MaxHealth += amount;
	AddHealth(amount);
}

void UHealthComponent::AddHealth(float amount)
{
	CurrentHealth += amount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::RemoveMaxHealth(float amount)
{
	MaxHealth -= amount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::Death()
{
	//Apply death logic
}

float UHealthComponent::GetHealthRatio()
{
	return CurrentHealth / MaxHealth;
}


