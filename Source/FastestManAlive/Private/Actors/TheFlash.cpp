// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "Actors/TheFlash.h"

// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#include "VFX/AfterimageVFX.h"
#include "EnhancedInputComponent.h"
#include "Components/SpeedsterStats.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpeedForceCombat.h"
#include "Kismet/KismetMathLibrary.h"

class UEnhancedInputLocalPlayerSubsystem;

ATheFlash::ATheFlash() {
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleCollider->SetVisibility(true);
	CapsuleCollider->SetHiddenInGame(true);
	CapsuleCollider->SetCapsuleHalfHeight(100);
	CapsuleCollider->SetCapsuleRadius(40);
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(this->GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	TargetLock = CreateDefaultSubobject<UTargetLock>(TEXT("Target Lock"));
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping"));
	Stats = CreateDefaultSubobject<USpeedsterStats>(TEXT("Speedster Stats"));
	Combat = CreateDefaultSubobject<USpeedForceCombat>(TEXT("Speed Force Combat"));
	AfterimageVfx = CreateDefaultSubobject<UAfterimageVFX>(TEXT("Afterimage VFX"));
	SmearVfx = CreateDefaultSubobject<USmearVFX>(TEXT("Smear VFX"));
	SpeedForce = CreateDefaultSubobject<USpeedForce>(TEXT("Speed Force"));
}

void ATheFlash::BeginPlay() {
	Super::BeginPlay();
	AddCrosshair();
}

void ATheFlash::AddCrosshair() {
	if (CrosshairWidgetClass) {
		CrosshairWidget = CreateWidget<UUserWidget>(GetController<APlayerController>(), CrosshairWidgetClass);
		check(CrosshairWidget.Get());
		CrosshairWidget->AddToViewport();
	}
}


void ATheFlash::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	SpeedForceMoveCharacter();
	// SmearVfx->Update(DeltaTime, SpeedForce->GetIsUsing());
	AfterimageVfx->Update(DeltaTime, SpeedForce->GetIsUsing());

	USpeedsterStats* SpeedsterStats = dynamic_cast<USpeedsterStats*>(Stats);
	if (IsValid(SpeedsterStats)) {
		SpeedsterStats->Update(DeltaTime);
	}
}

void ATheFlash::SpeedForceMoveCharacter() {
	const bool IsMoving = UKismetMathLibrary::NotEqual_Vector2DVector2D(MoveAxis, FVector2d::Zero(), 0.1);
	if (!IsMoving) {
		if (Stats->GetIsSprint()) {
			const FVector ControlRotation = UKismetMathLibrary::Conv_RotatorToVector(GetControlRotation());
			const float NegRand = UKismetMathLibrary::InRange_FloatFloat(
				                      ControlRotation.Z, -1, -0.1, true, true
			                      )
				                      ? ControlRotation.Z
				                      : 0;
			const float PosRand = UKismetMathLibrary::InRange_FloatFloat(
				                      ControlRotation.Z, 0.1, 1, true, true
			                      )
				                      ? ControlRotation.Z
				                      : 0;
			const FVector WorldDirection(ControlRotation.X, ControlRotation.Y, PosRand + NegRand);
			AddMovementInput(WorldDirection, 1.0, true);
		}
	}
}


void ATheFlash::MoveCharacter() {
	FVector FallBack = UKismetMathLibrary::Conv_RotatorToVector(GetControlRotation());
	const bool PosChecker = UKismetMathLibrary::InRange_FloatFloat(FallBack.Z, 0.1f, 1.0f, true, true);
	const bool NegChecker = UKismetMathLibrary::InRange_FloatFloat(
		FallBack.Z, -1.0f, -0.1f, true, true);
	const float Pos = PosChecker ? FallBack.Z : 0;
	const float Neg = NegChecker ? FallBack.Z : 0;
	FallBack.Z = Neg + Pos;
	const FVector WorldDirectionY = SpeedForce->GetIsUsing() ? FallBack : GetInputAngles(true);
	const FVector WorldDirectionX = SpeedForce->GetIsUsing() ? FVector::Zero() : GetInputAngles(false);
	const float ScaleY = SpeedForce->GetIsUsing() ? 1 : MoveAxis.Y;
	AddMovementInput(WorldDirectionY, ScaleY, false);
	AddMovementInput(WorldDirectionX, MoveAxis.X, false);
}

void ATheFlash::MoveTriggered(const FInputActionValue& Value) {
	Super::MoveTriggered(Value);
}

void ATheFlash::MoveCompleted(const FInputActionValue& Value) {
	MoveAxis = FVector2d::Zero();
	if (!CheckMovementMode(MOVE_Flying) && IsSprintBtnPressed) {
		IsSprintBtnPressed = false;
	}
}

void ATheFlash::AimTriggered() {
	Super::AimTriggered();
	if (!DidSpeedForceVFX) {
		DoOnceSpeedForceVFX();
		DidSpeedForceVFX = true;
	}
}

void ATheFlash::DoOnceSpeedForceVFX() {
	USpeedsterStats* SpeedsterStats = dynamic_cast<USpeedsterStats*>(Stats);
	SpeedsterStats->SetSpeedForceVFX(false, true);
	SpeedForce->SetIsCharging(true);
}

void ATheFlash::AimCompleted() {
	Super::AimCompleted();
	if (!SpeedForce->GetIsUsing()) {
		USpeedsterStats* SpeedsterStats = dynamic_cast<USpeedsterStats*>(Stats);
		SpeedsterStats->SetSpeedForceVFX(false, false);
		DidSpeedForceVFX = false;
		SpeedForce->SetIsCharging(false);
	}
}

void ATheFlash::DodgeTriggered() {
	Super::DodgeTriggered();
	// if (Stats->GetIsSprint()) {
	// 	Stats->DodgeEvent(InputDirection);
	// }
}

FVector ATheFlash::TargetSocket() {
	return Stats->GetIsAiming()
		       ? AimingSocketOffset
		       : (SpeedForce->GetIsUsing()
			          ? SpeedForce->GetSpeedForceSocketOffset()
			          : (Stats->GetIsSprint() ? SprintSocketOffset : DefaultSocketOffset));
}

float ATheFlash::TargetArmLength() {
	return Super::TargetArmLength();
}

void ATheFlash::SpeedForceToggleStarted() {
	SpeedForce->SetIsUsing(!SpeedForce->GetIsUsing());
}

void ATheFlash::SetupActions(UEnhancedInputComponent* EnhancedInputComponent) {
	Super::SetupActions(EnhancedInputComponent);
	EnhancedInputComponent->BindAction(SpeedForceToggleAction, ETriggerEvent::Started, this,
	                                   &ATheFlash::SpeedForceToggleStarted);
}
