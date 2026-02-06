// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtBlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AArtBlackholeProjectile::AArtBlackholeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// EVERYTHING ELSE AS IN BASE CLASS CONSTRUCTOR

	MovementComponent->InitialSpeed = 1000.0f;

	SphereComponent->SetCollisionProfileName("Blackhole");

	FatalZoneSphereComponent = CreateDefaultSubobject<USphereComponent>("FatalZone");
	FatalZoneSphereComponent->SetCollisionProfileName("Blackhole");
	FatalZoneSphereComponent->SetupAttachment(SphereComponent);

	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComponent->SetupAttachment(SphereComponent);
}

void AArtBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AArtBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

