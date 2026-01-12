// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtHealthPotion.h"
#include "Components/SphereComponent.h"
#include "ArtAttributeComponent.h"

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
	if (!AttributeComp->IsFullHealth())
	{
		AttributeComp->ApplyHealthChange(this, DeltaHealth);
		MakeUninteractable();
	}
}

void AArtHealthPotion::MakeUninteractable()
{
	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	///\TODO: fix magic number
	GetWorldTimerManager().SetTimer(TimerHandle_SetVisibility, this, &AArtHealthPotion::SetVisibility_TimeElapsed, 10.0f);
}

void AArtHealthPotion::SetVisibility_TimeElapsed()
{
	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

