// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCGSettings.h"
#include "GetAllRoadData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup = (Procedural))
class DREADNIGHT_API UPCG_GetAllRoadData :  public UPCGSettings
{
	GENERATED_BODY()

public:
	UPCG_GetAllRoadData();

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

	/** Number of points to sample per spline segment for better curve representation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings", meta = (PCG_Overridable, ClampMin = "2", ClampMax = "100"))
	int32 SamplesPerSegment = 10;

	/** If true, merge all spline data into a single output. If false, output separate data for each spline */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings", meta = (PCG_Overridable))
	bool bMergeSplines = true;
};

/**
 * Custom PCG Element that executes the spline gathering logic
 */
class FPCGExcludeSplineElement : public IPCGElement
{
protected:
	virtual bool ExecuteInternal(FPCGContext* Context) const override;

	virtual bool CanExecuteOnlyOnMainThread(FPCGContext* Context) const override;
};