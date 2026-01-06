#include "Actors/Traps/RotativeBladeTrap.h"
#include "Items/Data/BuildingDataAsset.h"
#include "GameFramework/RotatingMovementComponent.h"

ARotativeBladeTrap::ARotativeBladeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("PillarMesh");

	PillarMesh->SetupAttachment(RootComponent);
}

void ARotativeBladeTrap::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARotativeBladeTrap::OnBladeBeginOverlap);

	RotatingComponent->Deactivate();

	AnimateHeight = true;
	AnimateScale = false;
	CurrentSpawnHeight = PillarMesh->GetRelativeLocation().Z;
	Mesh->SetRelativeScale3D(FVector::ZeroVector);
}

void ARotativeBladeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimateRotativeBladeTrap(DeltaTime);
}

void ARotativeBladeTrap::ActivateTrap()
{
	RotatingComponent->RotationRate = FRotator(0.f, TrapData->RotationSpeed, 0.f);

	RotatingComponent->Activate();
}

void ARotativeBladeTrap::AnimateRotativeBladeTrap(float DeltaTime)
{
	if (AnimateHeight)
	{
		CurrentSpawnHeight = FMath::FInterpTo(CurrentSpawnHeight, FinishSpawnHeight, DeltaTime, LerpHeightSpeed);

		FVector NewPos = PillarMesh->GetRelativeLocation();
		NewPos.Z = CurrentSpawnHeight;

		PillarMesh->SetRelativeLocation(NewPos);

		if (FMath::Abs(FinishSpawnHeight) / FMath::Abs(CurrentSpawnHeight) > 0.90f)
			AnimateScale = true;

		if (CurrentSpawnHeight == FinishSpawnHeight)
			AnimateHeight = false;
		
	}
	if (AnimateScale)
	{
		CurrentScale = FMath::FInterpTo(CurrentScale, FinishScale, DeltaTime, LerpScaleSpeed);

		FVector NewScale = Mesh->GetRelativeScale3D();
		NewScale.X = CurrentScale;
		NewScale.Y = CurrentScale;
		NewScale.Z = CurrentScale;

		Mesh->SetRelativeScale3D(NewScale);

		if (CurrentScale == FinishScale)
			AnimateScale = false;
	}
}

void ARotativeBladeTrap::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseAICharacter * BaseAICharacter = Cast<ABaseAICharacter>(OtherActor))
	{
		BaseAICharacter->TryApplyDamage(TrapData->TrapDamage, this);
	}
}
