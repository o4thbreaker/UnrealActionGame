// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtAttributeComponent.h"

UArtAttributeComponent::UArtAttributeComponent()
{
	MaxHealth = 100;

	Health = MaxHealth;

	FMath::Clamp(Health, 0, MaxHealth);
}

void UArtAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UArtAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool UArtAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UArtAttributeComponent::IsFullHealth() const
{
	return MaxHealth <= Health;
}

