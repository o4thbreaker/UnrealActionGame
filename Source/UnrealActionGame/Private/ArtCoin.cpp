// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtCoin.h"
#include "ArtPlayerState.h"

void AArtCoin::Interact_Implementation(APawn* InstitgatorPawn)
{
	if (!ensure(InstitgatorPawn))
	{
		return;
	}

	AArtPlayerState* PlayerState = Cast<AArtPlayerState>(InstitgatorPawn->GetPlayerState());
	if (PlayerState)
	{
		GainCoin(PlayerState);
	}
}

void AArtCoin::GainCoin(AArtPlayerState* PlayerState)
{
	PlayerState->SetCoinsAmount(PlayerState->GetCoinsAmount() + 1);
	PlayerState->OnCoinsValueChanged.Broadcast(PlayerState->GetCoinsAmount());
	HideItem();
}
