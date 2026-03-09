// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerController.h"

void AArtPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void AArtPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void AArtPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}
