// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAction.h"
#include "ArtActionComponent.h"

void UArtAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UArtActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	IsRunning = true;
}

void UArtAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	// to check if IsRunning is true, otherwise smth is wrong
	ensureAlways(IsRunning);

	UArtActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	IsRunning = false;
}

UArtActionComponent* UArtAction::GetOwningComponent() const
{
	return Cast<UArtActionComponent>(GetOuter());
}

bool UArtAction::CanStart_Implementation(AActor* Instigator)
{
	if (GetIsRunning())
	{
		return false;
	}

	UArtActionComponent* Component = GetOwningComponent();

	if (Component->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
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

bool UArtAction::GetIsRunning() const
{
	return IsRunning;
}
