// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "Components/SpeedForceCombat.h"
#include "GameFramework/Character.h"


void USpeedForceCombat::BeginPlay() {
	Super::BeginPlay();

	SpeedForce = Character->GetComponentByClass<USpeedForce>();
}

void USpeedForceCombat::Attack(UAnimMontage* Montage) {
	AddStrikeWarpTarget();
	if (!GetIsAttacking()) {
		IsAttacking = true;
		Character->GetMesh()->AnimScriptInstance->Montage_Play(Montage);
		FOnMontageBlendingOutStarted BlendingOutDelegate;
		BlendingOutDelegate.BindUObject(this, &UCombat::StrikeBlendOut);
		Character->GetMesh()->AnimScriptInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, Montage);
		SpeedForce->SetIsUsing(true);
	}
}

void USpeedForceCombat::StrikeBlendOut(UAnimMontage* AnimMontage, bool Interrupted) {
	Super::StrikeBlendOut(AnimMontage, Interrupted);
	if (!Interrupted) {
		SpeedForce->SetIsUsing(false);
	}
}
