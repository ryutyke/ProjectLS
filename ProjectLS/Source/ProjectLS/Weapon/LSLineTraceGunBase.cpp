// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSLineTraceGunBase.h"

ALSLineTraceGunBase::ALSLineTraceGunBase()
{
}

void ALSLineTraceGunBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ALSLineTraceGunBase::Attack()
{
	if (!Super::Attack()) return false;



	return true;
}

bool ALSLineTraceGunBase::Reload()
{
	if(!Super::Reload()) return false;

	
	return true;
}
