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

bool UArtAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

float UArtAttributeComponent::GetHealth() const
{
	return Health;
}

float UArtAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UArtAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

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


UArtAttributeComponent* UArtAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UArtAttributeComponent>(FromActor->GetComponentByClass(UArtAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UArtAttributeComponent::IsActorAlive(AActor* Actor)
{
	UArtAttributeComponent* AttributeComponent = GetAttributes(Actor);

	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

