// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/Task/GetAllRoadData.h" 
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
UPCG_GetAllRoadData::UPCG_GetAllRoadData()
{
	ActorTag = NAME_None;
	SamplesPerSegment = 10;
	bMergeSplines = true;
}

#if WITH_EDITOR
FText UPCG_GetAllRoadData::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Exclude Spline Data");
}

FText UPCG_GetAllRoadData::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Gathers all AExcludeSpline actors from the world and converts their spline data to PCG spatial data for exclusion operations.");
}
#endif

TArray<FPCGPinProperties> UPCG_GetAllRoadData::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	// No input required - we gather from the world
	return PinProperties;
}

TArray<FPCGPinProperties> UPCG_GetAllRoadData::OutputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	PinProperties.SetNum(1);

	PinProperties[0].Label = PCGPinConstants::DefaultOutputLabel;
	PinProperties[0].AllowedTypes = EPCGDataType::Spatial;
	PinProperties[0].bAllowMultipleData = true;

	return PinProperties;
}

FPCGElementPtr UPCG_GetAllRoadData::CreateElement() const
{
	return MakeShared<FPCGExcludeSplineElement>();
}

// Element execution
bool FPCGExcludeSplineElement::ExecuteInternal(FPCGContext* Context) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGExcludeSplineElement::Execute);
	check(Context);

	if (!IsInGameThread())
		return false;
	
	const UPCG_GetAllRoadData* Settings = Context->GetInputSettings<UPCG_GetAllRoadData>();
	check(Settings);

	// Get the world from the PCG component
	UWorld* World = Context->ExecutionSource.Get() ? Context->ExecutionSource.GetObject()->GetWorld() : nullptr;
	if (!World)
	{
		PCGE_LOG(Error, GraphAndLog, LOCTEXT("NoWorld", "Unable to get world from PCG component"));
		return true;
	}

	TArray<UPCGPointData*> OutputPointDataArray;
	bool bSearchOnlySplineWithTag = Settings->ActorTag != NAME_None;
	
	// Iterate through all actors in the world looking for AExcludeSpline
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

		AExcludeSpline* ExcludeSpline = Cast<AExcludeSpline>(Actor);
		if (!ExcludeSpline)
		{
			continue;
		}
 
		if (bSearchOnlySplineWithTag && !Actor->ActorHasTag(Settings->ActorTag))
		{
			continue;
		}
		
		if (!ExcludeSpline->ExcludeSpline)
		{ 
			continue;
		}

		float RoadWidth = ExcludeSpline->RoadWidth;
		USplineComponent* SplineComponent = ExcludeSpline->ExcludeSpline;

		// Create PCG Point Data from spline
		UPCGPointData* PointData = NewObject<UPCGPointData>();
		TArray<FPCGPoint>& Points = PointData->GetMutablePoints();
		
		// Sample the spline and create points
		const int32 NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();
		const int32 TotalSamples = FMath::Max(2, (NumSplinePoints - 1) * Settings->SamplesPerSegment);
		const float SplineLength = SplineComponent->GetSplineLength();

		for (int32 i = 0; i <= TotalSamples; ++i)
		{
			const float Distance = (SplineLength * i) / TotalSamples;
			const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			const FVector Scale = FVector(1.0f, 1.0f, 1.0f);

			FPCGPoint& Point = Points.Emplace_GetRef();
			Point.Transform = FTransform(Rotation, Location, Scale);
			
			// Alternative simple: bounds cubiques autour du point
			const float HalfWidth = RoadWidth * 0.5f; 
			Point.BoundsMin = FVector(-HalfWidth, -HalfWidth, -HalfWidth);
			Point.BoundsMax = FVector(HalfWidth, HalfWidth, HalfWidth);
			
			Point.Density = 1.0f;
		}

		// Store actor name as metadata
		PointData->Metadata->CreateStringAttribute(TEXT("SourceActor"), Actor->GetName(), /*bAllowsInterpolation=*/false);
		PointData->Metadata->CreateFloatAttribute(TEXT("RoadWidth"), RoadWidth, /*bAllowsInterpolation=*/true);

		OutputPointDataArray.Add(PointData);
	}

	// Output the data
	if (OutputPointDataArray.Num() > 0)
	{
		if (Settings->bMergeSplines && OutputPointDataArray.Num() > 1)
		{
			// Merge all point data into one
			UPCGPointData* MergedData = NewObject<UPCGPointData>();
			TArray<FPCGPoint>& MergedPoints = MergedData->GetMutablePoints();

			for (UPCGPointData* Data : OutputPointDataArray)
			{
				MergedPoints.Append(Data->GetPoints());
			}

			FPCGTaggedData& TaggedData = Context->OutputData.TaggedData.Emplace_GetRef();
			TaggedData.Data = MergedData;
			TaggedData.Pin = PCGPinConstants::DefaultOutputLabel;
		}
		else
		{
			// Output each spline separately
			for (UPCGPointData* Data : OutputPointDataArray)
			{
				FPCGTaggedData& TaggedData = Context->OutputData.TaggedData.Emplace_GetRef();
				TaggedData.Data = Data;
				TaggedData.Pin = PCGPinConstants::DefaultOutputLabel;
			}
		}

		PCGE_LOG(Verbose, LogOnly, FText::Format(
			LOCTEXT("SplineDataGenerated", "Generated PCG data from {0} AExcludeSpline actor(s)"),
			FText::AsNumber(OutputPointDataArray.Num())
		));
	}
	else
	{
		PCGE_LOG(Warning, GraphAndLog, LOCTEXT("NoActorsFound", "No AExcludeSpline actors found in world"));
	}

	return true;
}

bool FPCGExcludeSplineElement::CanExecuteOnlyOnMainThread(FPCGContext* Context) const
{
	return true;
}

bool FPCGExcludeSplineElement::IsCacheable(const UPCGSettings* InSettings) const
{
	return false;
}

#undef LOCTEXT_NAMESPACE
