// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtBaseProjectile.h"
#include "GameplayTagContainer.h"
#include "ArtMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent; 
class UParticleSystem;
class UAudioComponent;
class UArtActionEffect;

UCLASS()
class UNREALACTIONGAME_API AArtMagicProjectile : public AArtBaseProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* FlightSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UArtActionEffect> BurningActionClass;

public:	
	// Sets default values for this actor's properties
	AArtMagicProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
