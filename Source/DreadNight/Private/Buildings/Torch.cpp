#include "Buildings/Torch.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Components/PointLightComponent.h"
#include "NiagaraComponent.h"

ATorch::ATorch()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	Light = CreateDefaultSubobject<UPointLightComponent>("Light");

	Light->SetupAttachment(Mesh);

	VFX = CreateDefaultSubobject<UNiagaraComponent>("VFX");

	VFX->SetupAttachment(Light);
}

void ATorch::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->RegisterLightSource(Light, VFX);

	if (GetWorld()->GetSubsystem<UDayCycleSubSystem>()->IsDay())
	{
		Light->SetIntensity(0);

		VFX->Deactivate();
	}
}

void ATorch::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->UnRegisterLightSource(Light, VFX);
}
