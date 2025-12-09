#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Global/BaseLevelWorldSettings.h"

void UWaveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InWorld.GetSubsystem<UDayCycleSubSystem>()->OnNightStart.AddDynamic(this, &UWaveWorldSubsystem::OnNightStart);
}

bool UWaveWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->GetMapName().Contains(Cast<ABaseLevelWorldSettings>(World->GetWorldSettings())->MapName);
}

void UWaveWorldSubsystem::OnNightStart()
{
	// TODO: Replace with SpawningSystem with Wave Data Asset

	FTimerHandle Handle;

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UWaveWorldSubsystem::EndNight, 5, false);
}

void UWaveWorldSubsystem::EndNight()
{
	// TODO: Replace with Monster death count

	OnWaveEnd.Broadcast();
}
