#include "Components/ArrowTrajectoryComponent.h"
#include "Kismet/GameplayStatics.h"

UArrowTrajectoryComponent::UArrowTrajectoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("TrajectorySpline"));
	Spline->SetMobility(EComponentMobility::Movable);
}


void UArrowTrajectoryComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


void UArrowTrajectoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AProjectileActor* Arrow = Cast<AProjectileActor>(GetOwner());

	ClearSpline();

	if (!Arrow->GetHasBeenShot())
	{

		FVector CameraLoc = PlayerCharacter->GetCamera()->GetComponentLocation();
		FVector CameraForward = PlayerCharacter->GetCamera()->GetForwardVector();
		FPredictProjectilePathParams PredictParams;
		PredictParams.StartLocation = GetOwner()->GetActorLocation();
		PredictParams.LaunchVelocity = (CameraForward * 1000 + CameraLoc) - PredictParams.StartLocation;
		PredictParams.LaunchVelocity.Normalize();
		PredictParams.LaunchVelocity *= Arrow->GetProjectileMovementComponent()->InitialSpeed;
		PredictParams.ProjectileRadius = 2.0f;
		PredictParams.MaxSimTime = 3.0f;
		PredictParams.DrawDebugType = EDrawDebugTrace::None;
		PredictParams.ActorsToIgnore.Add(GetOwner());
		PredictParams.bTraceWithCollision = false;
		PredictParams.bTraceWithChannel = true;
		PredictParams.TraceChannel = ECC_WorldStatic;
		PredictParams.bTraceComplex = false;
		PredictParams.DrawDebugTime = 1.0f;
		PredictParams.OverrideGravityZ = -490.f;

		FPredictProjectilePathResult PredictResult;
		UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

		int index = 0;
		for (int i = 0; i < PredictResult.PathData.Num(); i++)
		{
			Spline->AddSplinePoint(PredictResult.PathData[index].Location, ESplineCoordinateSpace::World);
			index = i;
		}
		Spline->SetSplinePointType(index, ESplinePointType::CurveClamped);

		for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 2; i++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->SetStaticMesh(SplineStaticMesh);
			SplineMesh->SetMaterial(0, SplineMat);
			SplineMesh->RegisterComponent();
			SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			SplineMeshes.Add(SplineMesh);

			FVector StartPos, StartTangent, EndPos, EndTangent;
			Spline->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
			Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);
			SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
		}
	}
}

void UArrowTrajectoryComponent::ClearSpline()
{
	for (USplineMeshComponent* Mesh : SplineMeshes)
	{
		Mesh->DestroyComponent();
	}
	SplineMeshes.Empty();
	if (Spline)
	{
		Spline->ClearSplinePoints();
	}
}