// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/Task/SelectExactPoint.h" 
#include "PCGContext.h"
#include "Data/PCGPointData.h"
#include "Data/PCGSplineData.h"
#include "Helpers/PCGHelpers.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PCG/ExcludeActor/ExcludeSpline.h"
#include "UObject/UnrealTypePrivate.h"

#define LOCTEXT_NAMESPACE "PCGExcludeSplineElement"

// Constructor
USelectExactPoint::USelectExactPoint()
{
}

#if WITH_EDITOR
FText USelectExactPoint::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Select Exact Point");
}

FText USelectExactPoint::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Select an exact number of points from an array.");
}
#endif

TArray<FPCGPinProperties> USelectExactPoint::InputPinProperties() const
{
    TArray<FPCGPinProperties> PinProperties;
    PinProperties.Emplace(PCGPinConstants::DefaultInputLabel, EPCGDataType::Point);
    return PinProperties;
}

TArray<FPCGPinProperties> USelectExactPoint::OutputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	PinProperties.SetNum(1);

	PinProperties[0].Label = PCGPinConstants::DefaultOutputLabel;
	PinProperties[0].AllowedTypes = EPCGDataType::Spatial;
	PinProperties[0].bAllowMultipleData = true;

	return PinProperties;
}

FPCGElementPtr USelectExactPoint::CreateElement() const
{
	return MakeShared<FUSelectExactPoint>();
}

// Element execution
bool FUSelectExactPoint::ExecuteInternal(FPCGContext* Context) const
{
    TRACE_CPUPROFILER_EVENT_SCOPE(FPCGSelectExactPointsElement::Execute);
    
    const USelectExactPoint* Settings = Context->GetInputSettings<USelectExactPoint>();
    check(Settings);

    TArray<FPCGTaggedData> Inputs = Context->InputData.GetInputsByPin(PCGPinConstants::DefaultInputLabel);
    TArray<FPCGTaggedData>& Outputs = Context->OutputData.TaggedData;

    for (const FPCGTaggedData& Input : Inputs)
    {
        const UPCGSpatialData* SpatialData = Cast<UPCGSpatialData>(Input.Data);
        if (!SpatialData)
        {
            continue;
        }

        const UPCGPointData* PointData = SpatialData->ToPointData(Context);
        if (!PointData)
        {
            continue;
        }

        const TArray<FPCGPoint>& InPoints = PointData->GetPoints();
        
        if (InPoints.Num() == 0)
        {
            continue;
        }

        UPCGPointData* OutPointData = NewObject<UPCGPointData>();
        OutPointData->InitializeFromData(PointData);
        TArray<FPCGPoint>& OutPoints = OutPointData->GetMutablePoints();
        OutPoints.Empty();

        int32 NumToSelect = FMath::Min(Settings->NumberOfPoint, InPoints.Num());
        
        if (Settings->bRandomSelection)
        {
            // Random selection using Fisher-Yates shuffle
            FRandomStream RandomStream(Settings->RandomSeed);
            TArray<int32> Indices;
            Indices.Reserve(InPoints.Num());
            
            for (int32 i = 0; i < InPoints.Num(); ++i)
            {
                Indices.Add(i);
            }
            
            // Shuffle only what we need
            for (int32 i = 0; i < NumToSelect; ++i)
            {
                int32 j = RandomStream.RandRange(i, InPoints.Num() - 1);
                Indices.Swap(i, j);
            }
            
            OutPoints.Reserve(NumToSelect);
            for (int32 i = 0; i < NumToSelect; ++i)
            {
                OutPoints.Add(InPoints[Indices[i]]);
            }
        }
        else
        {
            // Sequential selection (first N points)
            OutPoints.Reserve(NumToSelect);
            for (int32 i = 0; i < NumToSelect; ++i)
            {
                OutPoints.Add(InPoints[i]);
            }
        }

        FPCGTaggedData& Output = Outputs.Add_GetRef(Input);
        Output.Data = OutPointData;
    }

    return true;
}

bool FUSelectExactPoint::CanExecuteOnlyOnMainThread(FPCGContext* Context) const
{
	return true;
}

bool FUSelectExactPoint::IsCacheable(const UPCGSettings* InSettings) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE
