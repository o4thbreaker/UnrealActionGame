// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtBasePickUpItem.h"
#include "ArtAttributeComponent.h"

// Sets default values
AArtBasePickUpItem::AArtBasePickUpItem()
{
	AttributeComponent = CreateDefaultSubobject<UArtAttributeComponent>("AttributeComponent");
}

void AArtBasePickUpItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AArtBasePickUpItem::Interact_Implementation(APawn* InstitgatorPawn)
{
	// logic will be filled in derived classes
}


