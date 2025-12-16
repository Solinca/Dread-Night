#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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
	TObjectPtr<UImage> UpIconInformation;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DownIconInformation;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DistanceText;

	UPROPERTY(Transient)
	TObjectPtr<AActor> ObjectiveActor;

public:
	UFUNCTION(BlueprintCallable)
	void SetObjectiveActor(AActor* InObjectiveActor) { ObjectiveActor = InObjectiveActor; }
	AActor* GetObjectiveActor() const { return ObjectiveActor; }
	void SetDistanceText(const FText& InText) { DistanceText->SetText(InText); }
	void ShowUpInformationImage() const;
	void ShowDownInformationImage() const;
	void HideAllInformationImage() const;
};
