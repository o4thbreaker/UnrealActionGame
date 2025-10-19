// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class UNREALACTIONGAME_API AArtExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
