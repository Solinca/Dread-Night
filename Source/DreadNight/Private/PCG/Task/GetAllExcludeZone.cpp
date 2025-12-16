// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/Task/GetAllExcludeZone.h"
#include "PCGContext.h"
#include "Data/PCGPointData.h"
#include "Data/PCGSplineData.h"
#include "Helpers/PCGHelpers.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "PCG/ExcludeActor/ExcludeZone.h"
#include "UObject/UnrealTypePrivate.h"


#define LOCTEXT_NAMESPACE "PCGExcludeZoneElement"

UGetAllExcludeZone::UGetAllExcludeZone()
{
}

#if WITH_EDITOR
FText UGetAllExcludeZone::GetDefaultNodeTitle() const
{
	return LOCTEXT("NodeTitle", "Exclude Zone Data");
}

FText UGetAllExcludeZone::GetNodeTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Gathers all AExcludeZone actors from the world and converts their zone data to PCG spatial data for exclusion operations.");
}
#endif

TArray<FPCGPinProperties> UGetAllExcludeZone::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	// No input required - we gather from the world
	return PinProperties;
}

TArray<FPCGPinProperties> UGetAllExcludeZone::OutputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	PinProperties.SetNum(1);

	PinProperties[0].Label = PCGPinConstants::DefaultOutputLabel;
	PinProperties[0].AllowedTypes = EPCGDataType::Spatial;
	PinProperties[0].bAllowMultipleData = true;

	return PinProperties;
}

FPCGElementPtr UGetAllExcludeZone::CreateElement() const
{
	return MakeShared<FPCGGetAllExcludeZoneElement>();
}

bool FPCGGetAllExcludeZoneElement::ExecuteInternal(FPCGContext* Context) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGExcludeSplineElement::Execute);
	check(Context);

	if (!IsInGameThread())
		return false;
	
	const UGetAllExcludeZone* Settings = Context->GetInputSettings<UGetAllExcludeZone>();
	check(Settings);

	// Get the world from the PCG component
	UWorld* World = Context->ExecutionSource.Get() ? Context->ExecutionSource.GetObject()->GetWorld() : nullptr;
	if (!World)
	{
		PCGE_LOG(Error, GraphAndLog, LOCTEXT("NoWorld", "Unable to get world from PCG component"));
		return true;
	}
 
	bool bSearchOnlySplineWithTag = Settings->ActorTag != NAME_None;


	UPCGPointData* OutPointData = NewObject<UPCGPointData>();
	TArray<FPCGPoint>& OutPoints = OutPointData->GetMutablePoints();
	
	// Iterate through all actors in the world looking for AExcludeSpline
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

		AExcludeZone* ExcludeZone = Cast<AExcludeZone>(Actor);
		if (!ExcludeZone)
		{
			continue;
		}
 
		if (bSearchOnlySplineWithTag && !Actor->ActorHasTag(Settings->ActorTag))
		{
			continue;
		}
		
		if (!ExcludeZone->BlockingZone)
		{ 
			continue;
		}
 
		UBoxComponent* BoxComponent = ExcludeZone->BlockingZone;
        
		// Get box component transform and bounds
		FTransform BoxTransform = BoxComponent->GetComponentTransform();
		FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
        
		// Create a point representing this exclude zone
		FPCGPoint& Point = OutPoints.Emplace_GetRef();
		Point.Transform.SetLocation(BoxTransform.GetLocation());
		Point.Transform.SetRotation(BoxTransform.GetRotation());
		Point.SetLocalBounds(FBox(-BoxExtent, BoxExtent));
        
		// Store metadata about the exclude zone
		Point.Density = 1.0f;
		Point.Steepness = 1.f;
        
		// Optional: Store actor name as metadata
		Point.Seed = GetTypeHash(Actor->GetName());
	}

	// Set the bounds for the entire point data
	if (OutPoints.Num() > 0)
	{		
		FPCGTaggedData& TaggedData = Context->OutputData.TaggedData.Emplace_GetRef();
		TaggedData.Data = OutPointData;
		TaggedData.Pin = PCGPinConstants::DefaultOutputLabel;
	
        
		PCGE_LOG(Verbose, GraphAndLog, FText::Format(LOCTEXT("ExcludeZonesFound", "Found {0} exclude zone(s)"), OutPoints.Num()));
	}
	else
	{
		PCGE_LOG(Warning, GraphAndLog, LOCTEXT("NoExcludeZones", "No exclude zones found in the world"));
	}

	return true;
}

bool FPCGGetAllExcludeZoneElement::CanExecuteOnlyOnMainThread(FPCGContext* Context) const
{
	return true;
}

bool FPCGGetAllExcludeZoneElement::IsCacheable(const UPCGSettings* InSettings) const
{
	return false;
}


#undef LOCTEXT_NAMESPACE
