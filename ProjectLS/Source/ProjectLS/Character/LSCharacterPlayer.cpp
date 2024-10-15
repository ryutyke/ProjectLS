// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LSCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LSCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"

ALSCharacterPlayer::ALSCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectLS/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectLS/Input/Actions/IA_Move_Shoulder.IA_Move_Shoulder'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectLS/Input/Actions/IA_Look_Shoulder.IA_Look_Shoulder'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectLS/Input/Actions/IA_Crouch_Shoulder.IA_Crouch_Shoulder'"));
	if (nullptr != InputActionShoulderCrouchRef.Object)
	{
		ShoulderCrouchAction = InputActionShoulderCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectLS/Input/Actions/IA_Aim_Shoulder.IA_Aim_Shoulder'"));
	if (nullptr != InputActionShoulderAimRef.Object)
	{
		ShoulderAimAction = InputActionShoulderAimRef.Object;
	}

	// Curve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> StandToCrouchCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/ProjectLS/Curve/FC_StandToCrouch.FC_StandToCrouch'"));
	if (nullptr != StandToCrouchCurveRef.Object)
	{
		StandToCrouchCurve = StandToCrouchCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveVector> AimCurveRef(TEXT("/Script/Engine.CurveVector'/Game/ProjectLS/Curve/VC_Aim.VC_Aim'"));
	if (nullptr != AimCurveRef.Object)
	{
		AimCurve = AimCurveRef.Object;
	}


	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	StandToCrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("StandToCrouchTimeline"));
	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimeline"));
}

void ALSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);

	FOnTimelineFloat StandToCrouchCallback;
	StandToCrouchCallback.BindUFunction(this, FName("SetCameraBoomHeight"));
	StandToCrouchTimeline->SetLooping(false);
	StandToCrouchTimeline->AddInterpFloat(StandToCrouchCurve, StandToCrouchCallback);
	StandToCrouchTimeline->SetTimelineLength(0.35f);

	FOnTimelineVector AimCallback;
	AimCallback.BindUFunction(this, FName("SetCameraView"));
	AimTimeline->SetLooping(false);
	AimTimeline->AddInterpVector(AimCurve, AimCallback);
	AimTimeline->SetTimelineLength(0.1f);
}

void ALSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ALSCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ALSCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(ShoulderCrouchAction, ETriggerEvent::Started, this, &ALSCharacterPlayer::ShoulderStandToCrouch);
	EnhancedInputComponent->BindAction(ShoulderCrouchAction, ETriggerEvent::Completed, this, &ALSCharacterPlayer::ShoulderCrouchToStand);
	EnhancedInputComponent->BindAction(ShoulderAimAction, ETriggerEvent::Started, this, &ALSCharacterPlayer::ShoulderAim);
	EnhancedInputComponent->BindAction(ShoulderAimAction, ETriggerEvent::Completed, this, &ALSCharacterPlayer::ShoulderStopAiming);
}

void ALSCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	ULSCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ALSCharacterPlayer::SetCharacterControlData(const ULSCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
	CameraBoom->SocketOffset = CharacterControlData->SocketOffset;
	CameraBoom->SetRelativeLocation(CharacterControlData->SpringArmLocation);
}

void ALSCharacterPlayer::SetCameraBoomHeight(float Z)
{
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, Z));
}

void ALSCharacterPlayer::SetCameraView(FVector Vec)
{
	CameraBoom->SocketOffset.X = Vec.X;
	FollowCamera->FieldOfView = Vec.Y;
}

void ALSCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALSCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALSCharacterPlayer::ShoulderStandToCrouch()
{
	if (bIsCrouching) return;

	bIsCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	if (StandToCrouchTimeline->IsPlaying())
	{
		StandToCrouchTimeline->Play();
	}
	else
	{
		StandToCrouchTimeline->PlayFromStart();
	}
}

void ALSCharacterPlayer::ShoulderCrouchToStand()
{
	if (!bIsCrouching) return;

	bIsCrouching = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	
	if (StandToCrouchTimeline->IsPlaying())
	{
		StandToCrouchTimeline->Reverse();
	}
	else
	{
		StandToCrouchTimeline->ReverseFromEnd();
	}
}

void ALSCharacterPlayer::ShoulderAim()
{
	if (bIsAiming) return;

	bIsAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	if (AimTimeline->IsPlaying())
	{
		AimTimeline->Play();
	}
	else
	{
		AimTimeline->PlayFromStart();
	}
}

void ALSCharacterPlayer::ShoulderStopAiming()
{
	if (!bIsAiming) return;
	
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	if (AimTimeline->IsPlaying())
	{
		AimTimeline->Reverse();
	}
	else
	{
		AimTimeline->ReverseFromEnd();
	}
}

// ex) Start rolling when aiming
void ALSCharacterPlayer::ShoulderCancelAiming()
{
	if (!bIsAiming) return;

	
}

void ALSCharacterPlayer::ShoulderShoot()
{
	bIsShooting = true;
}

void ALSCharacterPlayer::ShoulderStopShooting()
{
	if (!bIsShooting) return;
}

void ALSCharacterPlayer::ShoulderCancelShooting()
{
	if (!bIsShooting) return;
}
