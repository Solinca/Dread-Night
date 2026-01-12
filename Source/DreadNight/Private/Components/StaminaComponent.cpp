#include "Components/StaminaComponent.h"

#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = PlayerData->MaxStamina;

	auto GameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	if (!GameInstance->IsNewGame())
	{
		GameInstance->OnPCGEndGeneration.AddDynamic(this, &UStaminaComponent::ActivateRegenAfterLoad);
	}
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanRegen)
	{
		RegenStamina(DeltaTime);
	}
}

void UStaminaComponent::AddStamina(float amount)
{
	CurrentStamina += amount;

	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, PlayerData->MaxStamina);

	OnStaminaChanged.Broadcast(CurrentStamina, PlayerData->MaxStamina);
}

void UStaminaComponent::RemoveStamina(float amount)
{
	CurrentStamina -= amount;

	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, PlayerData->MaxStamina);

	OnStaminaChanged.Broadcast(CurrentStamina, PlayerData->MaxStamina);
}

float UStaminaComponent::GetStaminaRatio()
{
	return CurrentStamina / PlayerData->MaxStamina;
}

void UStaminaComponent::RegenStamina(float DeltaTime)
{
	if (CurrentStamina < PlayerData->MaxStamina)
	{
		CurrentStamina += PlayerData->StaminaRegenPerSecond * DeltaTime;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, PlayerData->MaxStamina);

	OnStaminaChanged.Broadcast(CurrentStamina, PlayerData->MaxStamina);
}

void UStaminaComponent::SetCanRegen(bool value)
{
	CanRegen = value;
}

float UStaminaComponent::GetCurrentStamina()
{
	return CurrentStamina;
}
 
void UStaminaComponent::OnPostLoad()
{ 
	OnStaminaChanged.Broadcast(CurrentStamina, PlayerData->MaxStamina);
}

void UStaminaComponent::ActivateRegenAfterLoad()
{
	SetCanRegen(true);
}