// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "AnimNotify/Normal/BoltThrow.h"

#include "Components/SpeedForce.h"

void UBoltThrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference) {
	USpeedForce* SpeedForce = MeshComp->GetOwner()->GetComponentByClass<USpeedForce>();
	if (IsValid(SpeedForce)) {
		SpeedForce->ThrowLightningBolt();
	}
}
