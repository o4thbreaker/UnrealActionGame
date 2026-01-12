// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtBaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS(ABSTRACT)
class UNREALACTIONGAME_API AArtBaseProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* EffectComponent;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* ExplosionParticleEmmiter;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* ImpactSoundComponent;

public:	
	AArtBaseProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void DestroyProjectile();
};
