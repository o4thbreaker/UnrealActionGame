// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtTeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AArtTeleportProjectile::AArtTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EVERYTHING AS IN BASE CLASS CONSTRUCTOR
}

void AArtTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &AArtTeleportProjectile::OnSphereComponentHit);
}

// Called when the game starts or when spawned
void AArtTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_ProjectileExplosion, this, &AArtTeleportProjectile::ProjectileExplosion_TimeElapsed, 2.0f);
}

void AArtTeleportProjectile::ProjectileExplosion_TimeElapsed()
{
	MovementComponent->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(TimerHandle_InstigatorTeleportation, this, &AArtTeleportProjectile::InstigatorTeleportation_TimeElapsed, 0.2f);
}

void AArtTeleportProjectile::InstigatorTeleportation_TimeElapsed()
{
	FVector ProjectileLocation = GetActorLocation();

	if (ensure(ExplosionParticleEmmiter))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleEmmiter, GetActorLocation(), GetActorRotation());

	this->Destroy();

	GetInstigator()->SetActorLocation(ProjectileLocation);
}

void AArtTeleportProjectile::OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugString(GetWorld(), Hit.ImpactPoint, TEXT("TP_PRJCTL_HIT"), 0, FColor::Yellow, 0.5f);

	if (OtherActor != GetInstigator())
	{
		MovementComponent->StopMovementImmediately();

		if (GetWorldTimerManager().IsTimerActive(TimerHandle_ProjectileExplosion))
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_ProjectileExplosion);
		}

		InstigatorTeleportation_TimeElapsed();
	}
}

// Called every frame
void AArtTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

