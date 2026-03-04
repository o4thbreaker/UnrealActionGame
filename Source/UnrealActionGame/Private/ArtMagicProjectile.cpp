// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ArtAttributeComponent.h"
#include "ArtGameplayFunctionLibrary.h"
#include "ArtActionComponent.h"
#include "ArtActionEffect.h"

AArtMagicProjectile::AArtMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	FlightSoundComponent = CreateDefaultSubobject<UAudioComponent>("FlightSoundComponent");
	FlightSoundComponent->bAutoActivate = false;

	// EVERYTHING ELSE AS IN BASE CLASS CONSTRUCTOR
}

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
		UArtActionComponent* ActionComponent = Cast<UArtActionComponent>(OtherActor->GetComponentByClass(UArtActionComponent::StaticClass()));

		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UArtGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			DestroyProjectile();

			if (ActionComponent && HasAuthority())
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}

void AArtMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

