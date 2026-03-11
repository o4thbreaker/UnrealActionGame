// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtAction.h"
#include "ArtActionComponent.h"
#include "../UnrealActionGame.h"
#include "Net/UnrealNetwork.h"


void UArtAction::Initialize(UArtActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

void UArtAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UArtActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.IsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UArtAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	// to check if IsRunning is true, otherwise smth is wrong
	// BUUUUT it is not working for mp... the check is fine for server, but clients can stop when IsRunning = false (see OnRep_IsRunning())
	//ensureAlways(IsRunning);

	UArtActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.IsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

UArtActionComponent* UArtAction::GetOwningComponent() const
{
	return ActionComponent;
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
	AActor* Actor = Cast<AActor>(GetOuter());

	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

// server to clients: hey im now running, please also start
void UArtAction::OnRep_RepData()
{
	if (RepData.IsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UArtAction::GetIsRunning() const
{
	return RepData.IsRunning;
}

void UArtAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UArtAction, RepData);
	DOREPLIFETIME(UArtAction, TimeStarted);
	DOREPLIFETIME(UArtAction, ActionComponent);
}
