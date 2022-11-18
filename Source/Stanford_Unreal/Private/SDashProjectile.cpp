// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashProjectile::ASDashProjectile() {
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f;
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay() {
	Super::BeginPlay();
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayDetonate);
}

void ASDashProjectile::Explode_Implementation() {
	// Clear timer if the Explode was already called through another source like OnActorHit
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayDetonate, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	// Super::Explode_Implementation();
}

void ASDashProjectile::TeleportInstigator() {
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport)) {
		// Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

