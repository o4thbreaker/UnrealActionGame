// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ArtSaveGame.h"

void AArtPlayerState::OnRep_Coins(int32 OldCoinsAmount)
{
	// we can use OldCoinsAmount to figure out the delta if needed
	// but in this case CurrentCoinsAmount is already stores needed value

	OnCoinsValueChanged.Broadcast(CurrentCoinsAmount);
}

AArtPlayerState::AArtPlayerState()
{
	CurrentCoinsAmount = 0;
}

int32 AArtPlayerState::GetCoinsAmount() const
{
	return CurrentCoinsAmount;
}

void AArtPlayerState::SetCoinsAmount(int32 NewAmount)
{
	CurrentCoinsAmount = NewAmount;

	OnRep_Coins(CurrentCoinsAmount);
}

void AArtPlayerState::SavePlayerState_Implementation(UArtSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Coins = CurrentCoinsAmount;
	}
}

void AArtPlayerState::LoadPlayerState_Implementation(UArtSaveGame* SaveObject)
{
	if (SaveObject)
	{
		CurrentCoinsAmount = SaveObject->Coins;

		OnCoinsValueChanged.Broadcast(CurrentCoinsAmount);
	}
}

void AArtPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArtPlayerState, CurrentCoinsAmount);
}
