// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat.h"
#include "Components/TargetLock.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MotionWarpingComponent.h"
#include "Enumerators/EInputDirection.h"
#include "Components/BaseStats.h"
#include "BaseHero.generated.h"

UCLASS()
class FASTESTMANALIVE_API ABaseHero : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseHero();
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UCapsuleComponent* CapsuleCollider;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UTargetLock* TargetLock;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UCombat* Combat;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UBaseStats* Stats;
	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	UMotionWarpingComponent* MotionWarping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void MoveTriggered(const FInputActionValue& Value);
	virtual void MoveCompleted(const FInputActionValue& Value);
	virtual void LookTriggered(const FInputActionValue& Value);
	virtual void JumpStarted();
	virtual void JumpCompleted();
	virtual void DodgeTriggered();
	virtual void SprintStarted();
	virtual void AimTriggered();
	virtual void AimCompleted();
	virtual void TargetLockStarted();
	virtual void StrikeStarted();
	virtual void MoveCharacter();
	virtual void CheckInputDirection();
	void CheckGround();
	void UpdateHUD();
	void CheckHasMovementInput();
	void UpdateCamera();
	void ResetLookAtLocation();
	void ResetCameraPosition();
	void UpdateSpringArmSocketOffset();
	bool CheckCameraPosition();
	bool CheckMovementMode(EMovementMode MovementMode);
	FVector GetInputAngles(bool XY);
	FVector2d MoveAxis;
	FVector2d LookAxis;
	bool IsGrounded;
	bool IsSprintBtnPressed;
	bool IsAttacking;
	EInputDirection InputDirection;
	TWeakObjectPtr<UUserWidget> CrosshairWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero/Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	virtual FVector TargetSocket();
	virtual float TargetArmLength();

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	FVector DefaultLookAtLocationZ = FVector(0, 0, -2000);

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	FVector AimingSocketOffset = FVector(0, 100, 20);

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	FVector DefaultSocketOffset = FVector(160.0, 69, 30);

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	FVector SprintSocketOffset = FVector(60, 0, 10);

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float AimingArmLength = 200;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float DefaultArmLength = 300;

	bool DidResetCameraPosition;
	bool DidResetLookAtLocation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	UInputMappingContext* InputMappingContext;
	virtual void SetupActions(UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* TargetLockAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	const UInputAction* StrikeAction;
};
