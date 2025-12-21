// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtBasePickUpItem.h"
#include "ArtHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtHealthPotion : public AArtBasePickUpItem
{
	GENERATED_BODY()

protected:

	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

private:

	// dunno how to call it
	void SwitchTrigerrability();

	
};
