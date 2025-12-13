#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassMarker.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class DREADNIGHT_API UCompassMarker : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DistanceText;

	UPROPERTY(Transient)
	TObjectPtr<AActor> ObjectiveActor;

public:
	void SetObjectiveActor(AActor* InObjectiveActor);
	AActor* GetObjectiveActor() const { return ObjectiveActor; }
};
