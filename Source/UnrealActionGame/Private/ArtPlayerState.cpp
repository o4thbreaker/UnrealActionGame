// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerState.h"

AArtPlayerState::AArtPlayerState()
{
	CurrentCoinsAmount = 0;
}

int32 AArtPlayerState::GetCoinsAmount()
{
	return CurrentCoinsAmount;
}

void AArtPlayerState::SetCoinsAmount(int32 NewAmount)
{
	CurrentCoinsAmount = NewAmount;
}
