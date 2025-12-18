// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AArtBaseProjectile::AArtBaseProjectile()
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
	MovementComponent->InitialSpeed = 5000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	ImpactSoundComponent = CreateDefaultSubobject<UAudioComponent>("ImpactSoundComponent");
	ImpactSoundComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AArtBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AArtBaseProjectile::DestroyProjectile()
{
	if (ImpactSoundComponent)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundComponent->GetSound(), GetActorLocation(),
			1.0f, 1.0f, 0.0f, ImpactSoundComponent->AttenuationSettings);
	}

	this->Destroy();
}

