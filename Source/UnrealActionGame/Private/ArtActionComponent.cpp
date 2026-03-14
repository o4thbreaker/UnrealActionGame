// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtActionComponent.h"
#include "ArtAction.h"
#include "../UnrealActionGame.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_ART);

UArtActionComponent::UArtActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

}

void UArtActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// is server?
	if (GetOwner()->HasAuthority())
	{
		// we want this server only because we want to make array once and replication system will make a copy
		for (TSubclassOf<UArtAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UArtActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// need the copy because otherwise we would crash because of remove actions method
	TArray<UArtAction*> ActionsCopy = Actions;

	for (UArtAction* Action : ActionsCopy)
	{
		if (Action && Action->GetIsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}


void UArtActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//for (UArtAction* Action : Actions)
	//{
	//	FColor TextColor = Action->GetIsRunning() ? FColor::Blue : FColor::White;

	//	FString ActionMsg = FString::Printf(TEXT("[%s] Action %s : IsRunning: %s : Outer: %s"),
	//		*GetNameSafe(GetOwner()),
	//		*GetNameSafe(Action),
	//		Action->GetIsRunning() ? TEXT("true") : TEXT("false"),
	//		*GetNameSafe(Action->GetOuter()));

	//	LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	//}

}

void UArtActionComponent::AddAction(AActor* Instigator, TSubclassOf<UArtAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UArtAction* NewAction = NewObject<UArtAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{

		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->IsAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UArtActionComponent::RemoveAction(UArtAction* ActionToRemove)
{
	
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

			// is client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
		
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UArtActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	// start measuring everything down below the function
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (UArtAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->GetIsRunning())
			{
				// is client
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

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

void UArtActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UArtActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UArtActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UArtActionComponent, Actions);
}

// when we replicate something, Unreal will open an ActorChannel as sort of thread to send data from client to server
bool UArtActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// we have array of UObject, so we need to replicate them as subobjects
	for (UArtAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

