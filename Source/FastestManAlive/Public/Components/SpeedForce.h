// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TargetLock.h"
#include "Components/BaseStats.h"
#include "NiagaraComponent.h"
#include "Chaos/ChaosEngineInterface.h"
#include "SpeedForce.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpeedForceDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FASTESTMANALIVE_API USpeedForce : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpeedForce();
	TWeakObjectPtr<UTargetLock> TargetLock;
	TWeakObjectPtr<UBaseStats> Stats;
	TWeakObjectPtr<ACharacter> Character;
	TWeakObjectPtr<UCameraComponent> CameraComponent;
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	FSpeedForceDelegate SpeedForceDelegate;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* SonicBoom;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* HighSpeedStart;

private:
	bool IsCharging;
	bool IsUsing;
	float Stored = 0;
	float MaxCapacity = 4;
	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	FVector SpeedForceSocketOffset = FVector(0, 0, 20);
	TWeakObjectPtr<UNiagaraComponent> SonicBoomRef;
	TWeakObjectPtr<UNiagaraComponent> HighSpeedRef;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* LightningThrowMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* LightningIdleThrowMontage;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UClass* LightningBoltClass;
	UPROPERTY(EditAnywhere, Category = "VFX")
	float LightningBoltScale = 3;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	bool GetIsUsing(bool DontCheckAnimation = false);
	void SetIsUsing(bool Value);
	bool LightningThrowCapable();
	void PlayLightningThrow(bool IsIdle);
	bool GetIsCharging();
	void SetIsCharging(bool Value);

	float GetStored();
	void SetStored(float Value);
	float GetMaxCapacity();
	UNiagaraComponent* SpawnNiagaraSystem(UNiagaraSystem* System);
	FVector GetSpeedForceSocketOffset();
	void SpawnSonicBoomVFXEvent(bool Value);
	void ThrowLightningBolt();
};
