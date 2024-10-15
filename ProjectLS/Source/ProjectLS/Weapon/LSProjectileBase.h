// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSProjectileBase.generated.h"

UCLASS()
class PROJECTLS_API ALSProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSProjectileBase();

protected:
	virtual void BeginPlay() override;

};
