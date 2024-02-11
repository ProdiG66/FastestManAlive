// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LightningBolt.generated.h"

UCLASS()
class FASTESTMANALIVE_API ALightningBolt : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningBolt();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UParticleSystemComponent> ParticleSystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnProjectileStop(const FHitResult& HitResult);
	float Lifetime = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
