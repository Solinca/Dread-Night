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

void ATorch::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->UnRegisterLightSource(Light, VFX);
}
