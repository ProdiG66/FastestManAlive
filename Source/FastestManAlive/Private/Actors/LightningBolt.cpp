// Copyright (c) 2024 Jan Enri Arquero. All rights reserved.


#include "Actors/LightningBolt.h"

#include "Components/BoxComponent.h"


// Sets default values
ALightningBolt::ALightningBolt() {
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	Box->SetBoxExtent(FVector(46, 6, 6));
	RootComponent = Box;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	Capsule->SetRelativeRotation(FRotator(90, 0, 0));
	Capsule->SetCapsuleHalfHeight(46);
	Capsule->SetCapsuleRadius(7);
	Capsule->SetupAttachment(RootComponent);
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Particle Component"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->SetRelativeLocation(FVector(-50, 0, 0));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 6000;
	ProjectileMovement->MaxSpeed = 9000;
	ProjectileMovement->ProjectileGravityScale = 0;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALightningBolt::BeginPlay() {
	Super::BeginPlay();

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ALightningBolt::OnProjectileStop);
}

void ALightningBolt::OnProjectileStop(const FHitResult& HitResult) {
	UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
	Destroy();
}

// Called every frame
void ALightningBolt::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (Lifetime < 10) {
		Lifetime += DeltaTime;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Timedout"));
		Destroy();
	}
}
