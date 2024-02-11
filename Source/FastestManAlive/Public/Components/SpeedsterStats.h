// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpeedForce.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Chaos/ChaosEngineInterface.h"
#include "SpeedsterStats.generated.h"

UCLASS()
class FASTESTMANALIVE_API USpeedsterStats : public UBaseStats {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	USpeedsterStats();
	TWeakObjectPtr<USpeedForce> SpeedForce;
	UFUNCTION()
	void UpdateSpeedForceVFX();
	void SetSpeedForceVFX(bool IsActiveTrail, bool IsActiveShroud);
	UNiagaraComponent* GetLightningTrailVFX();
	UNiagaraComponent* GetLightningShroudVFX();
	UNiagaraComponent* GetLightningShroudUpperBodyVFX();
	float Remap(float Value, float InMin, float InMax, float OutMin, float OutMax);
	void Update(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UNiagaraComponent* SpawnSystemAttached(UNiagaraSystem* NiagaraSystem);
	virtual void SetIsSprint(bool Value) override;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* LightningTrail;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* LightningShroud;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* LightningShroudUpperBody;

private:
	TWeakObjectPtr<UNiagaraComponent> LightningTrailRef;
	TWeakObjectPtr<UNiagaraComponent> LightningShroudRef;
	TWeakObjectPtr<UNiagaraComponent> LightningShroudUpperBodyRef;
};
