// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtBasePickUpItem.h"
#include "ArtCoin.generated.h"

class AArtPlayerState;

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtCoin : public AArtBasePickUpItem
{
	GENERATED_BODY()
	
protected:
	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

private:
	void GainCoin(AArtPlayerState* PlayerState);
};
