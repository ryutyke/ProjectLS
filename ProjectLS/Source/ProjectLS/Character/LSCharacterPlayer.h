// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "Logging/LogMacros.h"
#include "LSCharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCurveFloat;
class UCurveVector;
class UTimelineComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class PROJECTLS_API ALSCharacterPlayer : public ALSCharacterBase
{
	GENERATED_BODY()
	
public:
	ALSCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class ULSCharacterControlData* CharacterControlData) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShoulderMoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShoulderLookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShoulderCrouchAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShoulderAimAction;

	ECharacterControlType CurrentCharacterControlType;

	// TimeLine
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> StandToCrouchTimeline;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> StandToCrouchCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> AimTimeline;

	// x = Camera Offset X, y = Camera FOV
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveVector> AimCurve;


	UFUNCTION()
	void SetCameraBoomHeight(float Z);
	UFUNCTION()
	void SetCameraView(FVector Vec);

protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void ShoulderStandToCrouch();
	void ShoulderCrouchToStand();
	void ShoulderAim();
	void ShoulderStopAiming();
	void ShoulderCancelAiming();
	void ShoulderShoot();
	void ShoulderStopShooting();
	void ShoulderCancelShooting();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
