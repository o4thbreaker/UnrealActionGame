// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtBasePickUpItem.h"
#include "Components/SphereComponent.h"
#include "ArtAttributeComponent.h"

AArtBasePickUpItem::AArtBasePickUpItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractCollisionComponent = CreateDefaultSubobject<USphereComponent>("InteractSphereCollisionComponent");

	InteractCollisionComponent->SetupAttachment(StaticMeshComponent);

	// if server spawns this item every client will spawn a copy
	SetReplicates(true);
}

void AArtBasePickUpItem::Interact_Implementation(APawn* InstitgatorPawn)
{
	// logic will be filled in derived classes
}

void AArtBasePickUpItem::HideItem()
{
	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	///\TODO: fix magic number
	GetWorldTimerManager().SetTimer(TimerHandle_SetVisibility, this, &AArtBasePickUpItem::SetVisibility_TimeElapsed, 10.0f);
}

void AArtBasePickUpItem::SetVisibility_TimeElapsed()
{
	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


