// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAction.h"

void UArtAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UArtAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
}

UWorld* UArtAction::GetWorld() const
{
	//thing we instantiated in ActionComponent when created new Action
	//basically it is ActionComponent itself
	UActorComponent* Component = Cast<UActorComponent>(GetOuter());

	if (Component)
	{
		return Component->GetWorld();
	}

	return nullptr;
}
