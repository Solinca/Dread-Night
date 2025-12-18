// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SavableActor.h"


void ISavableActor::OnPreSave()
{
}

// Add default functionality here for any ISavableObject functions that are not pure virtual.
void ISavableActor::OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache)
{
}
