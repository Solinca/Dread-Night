// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystem/Interface/Damageable.h"


// Add default functionality here for any IDamageable functions that are not pure virtual.
bool IDamageable::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	return false;
}
