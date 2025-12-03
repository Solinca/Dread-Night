// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UItemDataAsset;
class UItemInstance;
/**
 * 
 */
class DREADNIGHT_API FItemInstanceFactory
{
public:
	static UItemInstance* CreateItem(UItemDataAsset* ItemDataAsset, const int StartStackSize);
};
