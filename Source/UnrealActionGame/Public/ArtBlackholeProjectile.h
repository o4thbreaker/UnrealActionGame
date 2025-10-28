// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtBaseProjectile.h"
#include "ArtBlackholeProjectile.generated.h"

class USphereComponent;
class URadialForceComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class UNREALACTIONGAME_API AArtBlackholeProjectile : public AArtBaseProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtBlackholeProjectile();

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* FatalZoneSphereComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	URadialForceComponent* ForceComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
