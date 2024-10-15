// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "LSLineTraceGunBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLS_API ALSLineTraceGunBase : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSLineTraceGunBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool Attack() override;
	virtual bool Reload() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Performance, Meta = (AllowPrivateAccess = "true"))
	float BulletDamage;
};
