// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "VFX/AfterimageVFX.h"
#include "VFX/SmearVFX.h"
#include "BaseHero.h"
#include "InputAction.h"
#include "Components/SpeedForce.h"
#include "TheFlash.generated.h"

UCLASS()
class FASTESTMANALIVE_API ATheFlash : public ABaseHero {
	GENERATED_BODY()

public:
	ATheFlash();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupActions(UEnhancedInputComponent* EnhancedInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Hero/Components")
	USpeedForce* SpeedForce;
	UPROPERTY(EditAnywhere, Category = "Hero/Components VFX")
	USmearVFX* SmearVfx;
	UPROPERTY(EditAnywhere, Category = "Hero/Components VFX")
	UAfterimageVFX* AfterimageVfx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero/Enhanced Input")
	UInputAction* SpeedForceToggleAction;

protected:
	void AddCrosshair();
	virtual void BeginPlay() override;

	virtual void MoveTriggered(const FInputActionValue& Value) override;
	virtual void MoveCompleted(const FInputActionValue& Value) override;
	virtual void AimTriggered() override;
	virtual void AimCompleted() override;
	virtual void DodgeTriggered() override;
	virtual FVector TargetSocket() override;
	virtual float TargetArmLength() override;
	void SpeedForceToggleStarted();
	virtual void MoveCharacter() override;

	void SpeedForceMoveCharacter();

private:
	void DoOnceSpeedForceVFX();
	bool DidSpeedForceVFX;
};
