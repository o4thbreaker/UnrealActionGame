// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtBlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AArtBlackholeProjectile::AArtBlackholeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EVERYTHING ELSE AS IN BASE CLASS CONSTRUCTOR

	SphereComponent->SetCollisionProfileName("Blackhole");

	FatalZoneSphereComponent = CreateDefaultSubobject<USphereComponent>("FatalZone");
	FatalZoneSphereComponent->SetCollisionProfileName("Blackhole");
	FatalZoneSphereComponent->SetupAttachment(SphereComponent);

	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("ForceComponent");
	ForceComponent->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void AArtBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

