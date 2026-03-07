// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAction_BlackholeAttack.h"
#include "ArtActionComponent.h"
#include "ArtAttributeComponent.h"

bool UArtAction_BlackholeAttack::CanStart_Implementation(AActor* Instigator)
{
	if (Super::CanStart_Implementation(Instigator))
	{
		UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Instigator);

		if (AttributeComponent && AttributeComponent->GetRage() > BlackholeRageCost)
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not enough RAGE to cast Blackhole"));
			return false;
		}
	}
	
	return false;
}

void UArtAction_BlackholeAttack::StartAction_Implementation(AActor* Instigator)
{
	UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Instigator);

	if (AttributeComponent && Instigator->HasAuthority())
	{
		// negative to lose rage
		AttributeComponent->ApplyRageChange(Instigator, -BlackholeRageCost);
	}

	Super::StartAction_Implementation(Instigator);
}
