#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Global/MyGameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UWaveWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Cast<AMyGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->OnNightStart.AddDynamic(this, &UWaveSystem::OnNightStart);
}

bool UWaveWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->GetMapName().Contains(TEXT("TestDayNightCycle"));
}

void UWaveWorldSubsystem::OnNightStart()
{

}