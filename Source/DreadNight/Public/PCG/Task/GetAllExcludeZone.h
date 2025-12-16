// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCGSettings.h"
#include "GetAllExcludeZone.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UGetAllExcludeZone : public UPCGSettings
{
	GENERATED_BODY()
public:
	UGetAllExcludeZone();

	//~Begin UPCGSettings interface
#if WITH_EDITOR
	virtual FName GetDefaultNodeName() const override { return FName(TEXT("Get All Road Data")); }
	virtual FText GetDefaultNodeTitle() const override;
	virtual FText GetNodeTooltipText() const override;
	virtual EPCGSettingsType GetType() const override { return EPCGSettingsType::Spatial; }
#endif

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual TArray<FPCGPinProperties> OutputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings interface
public:
	/** Tag to filter specific AExcludeSpline actors. Leave empty to get all AExcludeSpline actors */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings", meta = (PCG_Overridable))
	FName ActorTag;
};

class FPCGGetAllExcludeZoneElement : public IPCGElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* Context) const override;

	virtual bool CanExecuteOnlyOnMainThread(FPCGContext* Context) const override;

	virtual bool IsCacheable(const UPCGSettings* InSettings) const override;
};