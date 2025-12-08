#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	TObjectPtr<UMaterialInterface> OutlineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	FText InteractionText;
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> OutlineMaterialInstance;
	
public:
	UMaterialInstanceDynamic* GetOutlineMaterial();
	const FText& GetInteractionPromptText() const;

protected:
	virtual void BeginPlay() override;
};
