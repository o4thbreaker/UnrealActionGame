// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerState.h"
#include "Net/UnrealNetwork.h"

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

void AArtPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArtPlayerState, CurrentCoinsAmount);
}
