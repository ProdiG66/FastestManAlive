// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "Components/SpeedsterStats.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


// Sets default values
USpeedsterStats::USpeedsterStats() {}

// Called when the game starts or when spawned
void USpeedsterStats::BeginPlay() {
	Super::BeginPlay();

	SpeedForce = Character->GetComponentByClass<USpeedForce>();
	LightningShroudUpperBodyRef = SpawnSystemAttached(LightningShroudUpperBody);
	LightningShroudRef = SpawnSystemAttached(LightningShroud);
	LightningTrailRef = SpawnSystemAttached(LightningTrail);

	SpeedForce->SpeedForceDelegate.AddDynamic(this, &USpeedsterStats::UpdateSpeedForceVFX);
}

UNiagaraComponent* USpeedsterStats::SpawnSystemAttached(UNiagaraSystem* NiagaraSystem) {
	return UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, Character->GetCapsuleComponent(),
	                                                    FName("none"),
	                                                    FVector::Zero(),
	                                                    FRotator::ZeroRotator,
	                                                    EAttachLocation::KeepRelativeOffset,
	                                                    false, false,
	                                                    ENCPoolMethod::None, true);
}

void USpeedsterStats::Update(float DeltaTime) {
	const float TargetAmount = Remap(SpeedForce->GetStored(), 0, SpeedForce->GetMaxCapacity(), 100, 600);
	// UE_LOG(LogTemp, Warning, TEXT("TargetAmount: %f"), TargetAmount);
	GetLightningShroudVFX()->SetFloatParameter(FName("Amount"), TargetAmount);
}

float USpeedsterStats::Remap(float Value, float InMin, float InMax, float OutMin, float OutMax) {
	const float ClampedValue = FMath::Clamp(Value, InMin, InMax);
	const float RemappedValue = OutMin + (ClampedValue - InMin) * (OutMax - OutMin) / (InMax - InMin);
	return RemappedValue;
}

void USpeedsterStats::UpdateSpeedForceVFX() {
	const bool IsActive = SpeedForce->GetIsUsing(true);
	// UE_LOG(LogTemp, Warning, TEXT("IsActive: %hs"), IsActive ? "True" : "False");
	SetActiveComponent(!IsActive, 0.4, LightningTrailRef.Get(), IsActive, IsActive);
	SetActiveComponent(!IsActive, 0.4, LightningShroudRef.Get(), IsActive, IsActive);
}

void USpeedsterStats::SetSpeedForceVFX(bool IsActiveTrail, bool IsActiveShroud) {
	SetActiveComponent(false, 0, LightningTrailRef.Get(), IsActiveTrail, IsActiveTrail);
	SetActiveComponent(false, 0, LightningShroudRef.Get(), IsActiveShroud, IsActiveShroud);
}

UNiagaraComponent* USpeedsterStats::GetLightningTrailVFX() {
	return LightningTrailRef.Get();
}

UNiagaraComponent* USpeedsterStats::GetLightningShroudVFX() {
	return LightningShroudRef.Get();
}

UNiagaraComponent* USpeedsterStats::GetLightningShroudUpperBodyVFX() {
	return LightningShroudUpperBodyRef.Get();
}

void USpeedsterStats::SetIsSprint(bool Value) {
	IsSprint = Value;
	if (IsValid(CharacterMovement.Get())) {
		SetCollisionHeight(CharacterMovement->MovementMode);
		if (!CharacterMovement->bOrientRotationToMovement) {
			CharacterMovement->bOrientRotationToMovement = true;
		}
		CharacterMovement->MaxWalkSpeed = GetIsSprint() ? (SpeedForce->GetIsUsing() ? 6000 : 1500) : 600;
		CharacterMovement->MaxAcceleration = GetIsSprint() ? (SpeedForce->GetIsUsing() ? 65536 : 4096) : 1000;
	}
	SprintDelegate.Broadcast();
}
