// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AArtMagicProjectile::AArtMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	/*SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);*/
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

}

// Called when the game starts or when spawned
void AArtMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

