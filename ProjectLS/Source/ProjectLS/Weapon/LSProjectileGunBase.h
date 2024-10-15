// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "LSProjectileGunBase.generated.h"

UCLASS()
class PROJECTLS_API ALSProjectileGunBase : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSProjectileGunBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ALSProjectileBase> ProjectileClass;
};
