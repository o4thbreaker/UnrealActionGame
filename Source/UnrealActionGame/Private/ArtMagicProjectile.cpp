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

	// EVERYTHING AS IN BASE CLASS CONSTRUCTOR
}

// Called when the game starts or when spawned
void AArtMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArtMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!(OtherActor == GetInstigator()))
	{
		// add code from blueprints
	}
}

// Called every frame
void AArtMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

