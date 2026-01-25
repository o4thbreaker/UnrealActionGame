// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ArtAttributeComponent.h"
#include "ArtGameplayFunctionLibrary.h"

// Sets default values
AArtMagicProjectile::AArtMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlightSoundComponent = CreateDefaultSubobject<UAudioComponent>("FlightSoundComponent");
	FlightSoundComponent->bAutoActivate = false;

	// EVERYTHING ELSE AS IN BASE CLASS CONSTRUCTOR
}

// Called when the game starts or when spawned
void AArtMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	

	/// \TODO: Figure out how to make a continious sound of flight
	//FlightSoundComponent->Play();
}

void AArtMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AArtMagicProjectile::OnActorOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &AArtMagicProjectile::OnHit);
}

void AArtMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!(OtherActor == GetInstigator()))
	{
		DestroyProjectile();
	}
}

void AArtMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//UE_LOG(LogTemp, Log, TEXT("Actor %s is overlaping with %s"), *GetInstigator()->GetName(), *OtherActor->GetName());
		/*UArtAttributeComponent* AttributeComponent = Cast<UArtAttributeComponent>(OtherActor->GetComponentByClass(UArtAttributeComponent::StaticClass()));
		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(GetInstigator(), -DamageAmount);

			DestroyProjectile();
		}*/

		if (UArtGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			DestroyProjectile();
		}
	}
}

// Called every frame
void AArtMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

