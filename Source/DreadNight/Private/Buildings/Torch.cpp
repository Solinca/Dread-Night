#include "Buildings/Torch.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Components/PointLightComponent.h"

ATorch::ATorch()
{
	PrimaryActorTick.bCanEverTick = false;

	Light = CreateDefaultSubobject<UPointLightComponent>("Light");

	Light->SetupAttachment(MeshComp);
}

void ATorch::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->RegisterLightSource(Light);
}
