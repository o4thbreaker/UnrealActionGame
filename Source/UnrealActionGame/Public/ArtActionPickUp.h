// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtBasePickUpItem.h"
#include "ArtActionPickUp.generated.h"

class UArtAction;
class UArtActionComponent;

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtActionPickUp : public AArtBasePickUpItem
{
	GENERATED_BODY()

protected:
	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TSubclassOf<UArtAction> ActionClass;

private:
	UFUNCTION()
	void GainAction(APawn* InstigatorPawn, UArtActionComponent* InstigatorActionComponent);
	
};
