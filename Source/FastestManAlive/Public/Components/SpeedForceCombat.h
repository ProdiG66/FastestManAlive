// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpeedForce.h"
#include "Components/Combat.h"
#include "SpeedForceCombat.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FASTESTMANALIVE_API USpeedForceCombat : public UCombat {
	GENERATED_BODY()

public:
	TWeakObjectPtr<USpeedForce> SpeedForce;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Attack(UAnimMontage* Montage) override;
	virtual void StrikeBlendOut(UAnimMontage* AnimMontage, bool Interrupted) override;
};
