// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "AnimNotify/States/LightningCharge.h"

#include "Components/SpeedsterStats.h"

void ULightningCharge::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference) {
	Duration = TotalDuration;
	StateTime = 0;
	USpeedsterStats* SpeedsterStats = MeshComp->GetOwner()->GetComponentByClass<USpeedsterStats>();
	if (IsValid(SpeedsterStats)) {
		SpeedsterStats->SetActiveComponent(false, 0, SpeedsterStats->GetLightningShroudUpperBodyVFX(), true, true);
		SpeedsterStats->GetLightningShroudUpperBodyVFX()->SetFloatParameter(FName("Amount"), 400);
	}
}

void ULightningCharge::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                  const FAnimNotifyEventReference& EventReference) {
	StateTime += FrameDeltaTime;
	const float NormalizedTime = StateTime / Duration;
	const float Amount = Remap(NormalizedTime, 0, 1, 900, 100);
	USpeedsterStats* SpeedsterStats = MeshComp->GetOwner()->GetComponentByClass<USpeedsterStats>();
	if (IsValid(SpeedsterStats)) {
		SpeedsterStats->GetLightningShroudUpperBodyVFX()->SetFloatParameter(FName("Amount"), Amount);
	}
}

void ULightningCharge::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference) {
	USpeedsterStats* SpeedsterStats = MeshComp->GetOwner()->GetComponentByClass<USpeedsterStats>();
	if (IsValid(SpeedsterStats)) {
		SpeedsterStats->SetActiveComponent(false, 0, SpeedsterStats->GetLightningShroudUpperBodyVFX(), false, false);
	}
}

float ULightningCharge::Remap(float Value, float InMin, float InMax, float OutMin, float OutMax) {
	const float ClampedValue = FMath::Clamp(Value, InMin, InMax);
	const float RemappedValue = OutMin + (ClampedValue - InMin) * (OutMax - OutMin) / (InMax - InMin);
	return RemappedValue;
}
