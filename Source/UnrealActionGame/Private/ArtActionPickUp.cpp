// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtActionPickUp.h"
#include "ArtAction.h"
#include "ArtActionComponent.h"

void AArtActionPickUp::Interact_Implementation(APawn* InstitgatorPawn)
{
	if (ensure(InstitgatorPawn))
	{
		UArtActionComponent* ActionComponent = Cast<UArtActionComponent>(InstitgatorPawn->GetComponentByClass(UArtActionComponent::StaticClass()));
		if (ActionComponent)
		{
			GainAction(InstitgatorPawn, ActionComponent);
		}
	}
}

void AArtActionPickUp::GainAction(APawn* InstigatorPawn, UArtActionComponent* InstigatorActionComponent)
{
	if (!InstigatorActionComponent->IsInActions(ActionClass))
	{
		InstigatorActionComponent->AddAction(InstigatorPawn, ActionClass);
		HideItem();
	}
}
