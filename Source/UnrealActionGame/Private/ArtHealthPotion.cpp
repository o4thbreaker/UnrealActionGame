// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtHealthPotion.h"
//#include "Components/SphereComponent.h"
#include "ArtAttributeComponent.h"
#include "ArtPlayerState.h"

void AArtHealthPotion::Interact_Implementation(APawn* InstitgatorPawn)
{
	if (!ensure(InstitgatorPawn))
	{
		return;
	}

	UArtAttributeComponent* InstigatorAttributeComp = InstitgatorPawn->FindComponentByClass<UArtAttributeComponent>();
	if (ensure(InstigatorAttributeComp))
	{
		AddHealth(InstigatorAttributeComp);
	}
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
