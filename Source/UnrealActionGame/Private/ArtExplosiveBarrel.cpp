// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AArtExplosiveBarrel::AArtExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
}

// Called when the game starts or when spawned
void AArtExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AArtExplosiveBarrel::OnHit);
}

void AArtExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
}

// Called every frame
void AArtExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

