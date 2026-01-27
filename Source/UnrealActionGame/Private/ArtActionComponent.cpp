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
		AddAction(ActionClass);
	}
	
}

void UArtActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UArtActionComponent::AddAction(TSubclassOf<UArtAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UArtAction* NewAction = NewObject<UArtAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
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

