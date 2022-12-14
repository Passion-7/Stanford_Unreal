// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	// Leaving this on applies small constant force via component 'tick' (optional)
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	// Optional, ignore mass of other objects (if false, the impulse strength will be much higher to push most objects depending on mass)
	ForceComp->bImpulseVelChange = true;
	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	// Binding either in constructor or in PostInitializeComponents() below
	// MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
	
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASExplosiveBarrel::PostInitializeComponents() {
	// Don't forget to call parent function
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explolsive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s, at gmae time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombineString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombineString, nullptr, FColor::Green, 2.0f, true);
}
