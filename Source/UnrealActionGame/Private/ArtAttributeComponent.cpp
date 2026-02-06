// Fill out your copyright notice in the Description page of Project Settings.

#include "ArtAttributeComponent.h"
#include "ArtGameModeBase.h"

UArtAttributeComponent::UArtAttributeComponent()
{
	MaxHealth = 100.0f;

	Health = MaxHealth;

	//FMath::Clamp(Health, 0, MaxHealth);

	MaxRage = 100.0f;

	Rage = 0.0f;

	RagePercentage = 10.0f;
}

void UArtAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UArtAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

void UArtAttributeComponent::GainRage(float DamageIntake)
{
	float DamagePercent = (FMath::Abs(DamageIntake) / 100.0f) * RagePercentage;

	Rage = FMath::Clamp(Rage + DamagePercent, 0.0f, MaxRage);
	OnRageChanged.Broadcast(this, Rage, Rage + DamagePercent);

	FString RageMsg = FString::Printf(TEXT("Gained rage. Current rage: %f"), Rage);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, RageMsg);
}

void UArtAttributeComponent::LoseRage(float Amount)
{
	Rage = FMath::Clamp(Rage - Amount, 0.0f, MaxRage);
	OnRageChanged.Broadcast(this, Rage, Amount);

	FString RageMsg = FString::Printf(TEXT("Lost rage. Current rage: %f"), Rage);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, RageMsg);
}

float UArtAttributeComponent::GetRage() const
{
	return Rage;
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
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f)
	{
		GainRage(ActualDelta);
	}

	// died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		AArtGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AArtGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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

