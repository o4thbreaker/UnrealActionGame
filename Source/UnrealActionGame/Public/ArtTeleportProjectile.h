// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtBaseProjectile.h"
#include "ArtTeleportProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class UNREALACTIONGAME_API AArtTeleportProjectile : public AArtBaseProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* ExplosionParticleEmmiter;

	FTimerHandle TimerHandle_ProjectileExplosion;
	FTimerHandle TimerHandle_InstigatorTeleportation;

public:	
	// Sets default values for this actor's properties
	AArtTeleportProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void PostInitializeComponents() override;

	void ProjectileExplosion_TimeElapsed();
	void InstigatorTeleportation_TimeElapsed();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
