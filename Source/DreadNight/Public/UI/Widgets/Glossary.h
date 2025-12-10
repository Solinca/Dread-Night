// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Glossary.generated.h"


class URetainerBox;
class UListView;
class UScrollBox;

UCLASS()
class DREADNIGHT_API UGlossary : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URetainerBox> RetainerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UListView> ListView;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnListScrolled(const float ItemOffset, const float DistanceRemaining) const;
};
