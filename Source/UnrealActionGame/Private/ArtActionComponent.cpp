// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtActionComponent.h"
#include "ArtAction.h"

UArtActionComponent::UArtActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}
void UArtActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UArtAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}

void UArtActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UArtActionComponent::AddAction(AActor* Instigator, TSubclassOf<UArtAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UArtAction* NewAction = NewObject<UArtAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->IsAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UArtActionComponent::RemoveAction(UArtAction* ActionToRemove)
{
	/// \BUG: crashes when enemy shoots. no burning effect?
	if (!ensure(ActionToRemove && !ActionToRemove->GetIsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool UArtActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UArtAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UArtActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UArtAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->GetIsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

bool UArtActionComponent::IsInActions(TSubclassOf<UArtAction> ActionClassToCheck) const
{
	for (UArtAction* Action : Actions)
	{
		if (Action->GetClass() == ActionClassToCheck)
		{
			return true;
		}
	}

	return false;
}

