// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtHealthPotion.h"
//#include "Components/SphereComponent.h"
#include "ArtAttributeComponent.h"
#include "ArtPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

void AArtHealthPotion::Interact_Implementation(APawn* InstitgatorPawn)
{
	if (!ensure(InstitgatorPawn))
	{
		return;
	}

	UArtAttributeComponent* InstigatorAttributeComp = UArtAttributeComponent::GetAttributes(InstitgatorPawn);
	if (ensure(InstigatorAttributeComp))
	{
		AddHealth(InstigatorAttributeComp);
	}
}

FText AArtHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UArtAttributeComponent* InstigatorAttributeComp = UArtAttributeComponent::GetAttributes(InstigatorPawn);

	if (InstigatorAttributeComp && InstigatorAttributeComp->IsFullHealth())
	{
		// NSLOCTEXT is a macro that creates a localized text
		// first parameter is like a collection/category
		// second is a key (unique)
		// third is text to display
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}
	
	// unforunately we cant use strings as localised
	// so we have to convert to FText 
	// Format == Printf in a way
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} coins. Restores health to MAX."), UsageCoinCost);
}

void AArtHealthPotion::AddHealth(UArtAttributeComponent* AttributeComp)
{
	AArtPlayerState* PlayerState = Cast<AArtPlayerState>(Cast<APawn>(AttributeComp->GetOwner())->GetPlayerState());
	
	if (PlayerState->GetCoinsAmount() < UsageCoinCost)
	{
		return;
	}

	if (!AttributeComp->IsFullHealth())
	{
		AttributeComp->ApplyHealthChange(this, DeltaHealth);
		PlayerState->SetCoinsAmount(PlayerState->GetCoinsAmount() - UsageCoinCost);
		PlayerState->OnCoinsValueChanged.Broadcast(PlayerState->GetCoinsAmount());
		HideItem();
	}
}


#undef LOCTEXT_NAMESPACE