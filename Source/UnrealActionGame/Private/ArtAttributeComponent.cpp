// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAttributeComponent.h"

// Sets default values for this component's properties
UArtAttributeComponent::UArtAttributeComponent()
{
	Health = 100;
}


// Called when the game starts
void UArtAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UArtAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

