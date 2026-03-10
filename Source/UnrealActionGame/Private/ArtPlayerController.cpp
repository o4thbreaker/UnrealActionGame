// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtPlayerController.h"
#include "Blueprint/UserWidget.h"

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

void AArtPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr; // feed it to the beast (UE garbage collector)

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);

	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AArtPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AArtPlayerController::TogglePauseMenu);

}
