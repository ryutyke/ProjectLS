// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ULSAnimInstance::ULSAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void ULSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ALSCharacterBase>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void ULSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());

		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}

	if (Owner)
	{
		PitchRotator = FRotator(0.f, 0.f, (Owner->GetBaseAimRotation().Pitch) / -5.0f);
		bIsCrouching = Owner->IsCrouching();
	}
}
