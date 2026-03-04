// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerController.h"

void AArtPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}
