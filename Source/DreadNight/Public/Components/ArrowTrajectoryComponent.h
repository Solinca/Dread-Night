#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/ProjectileActor.h"
#include "ArrowTrajectoryComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREADNIGHT_API UArrowTrajectoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UArrowTrajectoryComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory")
	TObjectPtr<USplineComponent> Spline = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USplineMeshComponent*> SplineMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory")
	TObjectPtr<UStaticMesh> SplineStaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory")
	TObjectPtr<UMaterial> SplineMat = nullptr;

public:
	UFUNCTION()
	void ClearSpline();

};