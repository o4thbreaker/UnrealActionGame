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
}

void AArtBasePickUpItem::Interact_Implementation(APawn* InstitgatorPawn)
{
	// logic will be filled in derived classes
}


