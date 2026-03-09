// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ArtSaveGame.h"

AArtPlayerState::AArtPlayerState()
{
	CurrentCoinsAmount = 0;
}

void AArtPlayerState::MulticastCoinsValueChanged_Implementation()
{
	OnCoinsValueChanged.Broadcast(CurrentCoinsAmount);
}

int32 AArtPlayerState::GetCoinsAmount() const
{
	return CurrentCoinsAmount;
}

void AArtPlayerState::SetCoinsAmount(int32 NewAmount)
{
	CurrentCoinsAmount = NewAmount;

	MulticastCoinsValueChanged();
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
	}
}

void AArtPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArtPlayerState, CurrentCoinsAmount);
}
