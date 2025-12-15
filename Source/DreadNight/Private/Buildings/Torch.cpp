#include "Buildings/Torch.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"

ATorch::ATorch()
{
	PrimaryActorTick.bCanEverTick = false;

	Light = CreateDefaultSubobject<UPointLightComponent>("Light");

	Light->SetupAttachment(MeshComp);

	VFX = CreateDefaultSubobject<UNiagaraComponent>("VFX");

	VFX->SetupAttachment(Light);
}

void ATorch::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->RegisterLightSource(Light, VFX);
}
