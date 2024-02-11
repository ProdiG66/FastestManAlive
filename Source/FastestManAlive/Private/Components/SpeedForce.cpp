// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "Components/SpeedForce.h"

#include "NiagaraFunctionLibrary.h"
#include "Actors/LightningBolt.h"
#include "Components/Flight.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USpeedForce::USpeedForce() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USpeedForce::BeginPlay() {
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	AnimInstance = Character->GetMesh()->GetAnimInstance();
	Stats = Character->GetComponentByClass<UBaseStats>();
	CameraComponent = Character->GetComponentByClass<UCameraComponent>();
	TargetLock = Character->GetComponentByClass<UTargetLock>();
	SonicBoomRef = SpawnNiagaraSystem(SonicBoom);
	HighSpeedRef = SpawnNiagaraSystem(HighSpeedStart);
}

void USpeedForce::SpawnSonicBoomVFXEvent(bool Value) {
	if (IsValid(SonicBoomRef.Get())) {
		// SonicBoomRef->SetActive(Value, true);
	}
	if (IsValid(HighSpeedRef.Get())) {
		HighSpeedRef->SetActive(Value, true);
	}
}

void USpeedForce::ThrowLightningBolt() {
	const FTransform HandTransform = Character->GetMesh()->GetBoneTransform("hand_r");
	const FVector TargetLocation = GetOwner()->GetActorLocation() + (CameraComponent->GetForwardVector() * 3000.0f);
	const FRotator HandRotation = (TargetLocation - HandTransform.GetLocation()).Rotation();
	const FTransform SpawnTransform(HandRotation, HandTransform.GetLocation(), FVector(LightningBoltScale));
	GetWorld()->SpawnActor<AActor>(LightningBoltClass, SpawnTransform);
}

// Called every frame
void USpeedForce::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetIsUsing() || GetIsCharging()) {
		if (Stored < MaxCapacity) {
			Stored += DeltaTime;
		}
		else {
			Stored = MaxCapacity;
		}
	}
}

bool USpeedForce::GetIsCharging() {
	return IsCharging;
}

void USpeedForce::SetIsCharging(bool Value) {
	IsCharging = Value;
	if (!IsCharging) {
		SetStored(0);
	}
}

bool USpeedForce::GetIsUsing(bool DontCheckAnimation) {
	const bool AnimationCheck = AnimInstance->Montage_IsPlaying(LightningThrowMontage) || AnimInstance->
		Montage_IsPlaying(LightningIdleThrowMontage);
	return IsUsing || (DontCheckAnimation ? false : AnimationCheck);
}

void USpeedForce::SetIsUsing(bool Value) {
	IsUsing = Value;
	if (IsValid(Stats.Get())) {
		if (!IsUsing && LightningThrowCapable()) {
			PlayLightningThrow(false);
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]() {
				Stats->SetSprintEvent(IsUsing);
				SetStored(0);
			});
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.6, false);
		}
		else {
			Stats->SetSprintEvent(IsUsing);
			if (!AnimInstance->Montage_IsPlaying(LightningThrowMontage) && !AnimInstance->Montage_IsPlaying(
				LightningIdleThrowMontage)) {
				SetStored(0);
			}
		}
	}
	SpeedForceDelegate.Broadcast();
	SpawnSonicBoomVFXEvent(true);
}

bool USpeedForce::LightningThrowCapable() {
	return GetStored() >= MaxCapacity * 0.3;
}

void USpeedForce::PlayLightningThrow(bool IsIdle) {
	AnimInstance->Montage_Play(IsIdle ? LightningIdleThrowMontage : LightningThrowMontage);
}

float USpeedForce::GetStored() {
	return Stored;
}

void USpeedForce::SetStored(float Value) {
	Stored = Value;
}

float USpeedForce::GetMaxCapacity() {
	return MaxCapacity;
}

UNiagaraComponent* USpeedForce::SpawnNiagaraSystem(UNiagaraSystem* System) {
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		System, Character->GetMesh(),
		FName("pelvis"),
		FVector::Zero(),
		FRotator(90, 0, 0),
		EAttachLocation::KeepRelativeOffset,
		false, false,
		ENCPoolMethod::None, true
	);
}

FVector USpeedForce::GetSpeedForceSocketOffset() {
	return SpeedForceSocketOffset;
}
