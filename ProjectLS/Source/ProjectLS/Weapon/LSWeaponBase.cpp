// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSWeaponBase.h"

ALSWeaponBase::ALSWeaponBase()
{

}

void ALSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ALSWeaponBase::Attack()
{
	if (!CanAttack()) return false;

	return true;
}

bool ALSWeaponBase::Reload()
{
	if (IsAmmoFull())
	{
		UE_LOG(LogTemp, Log, TEXT("Reloading When Full Ammo"));
		return false;
	}

	return true;
}

bool ALSWeaponBase::CanAttack() const
{
	if (bNeedAmmoToShoot && !HasAmmo())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack When No Ammo"));
		return false;
	}

	// fire rate
	// weapon on hand ready

	return true;
}

