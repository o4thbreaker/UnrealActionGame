// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtBasePickUpItem.h"
#include "ArtHealthPotion.generated.h"

class UArtAttributeComponent;

UCLASS()
class UNREALACTIONGAME_API AArtHealthPotion : public AArtBasePickUpItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Health")
	float DeltaHealth = 50.0f;

	FTimerHandle TimerHandle_SetVisibility;

protected:
	virtual void Interact_Implementation(APawn* InstitgatorPawn) override;

private:
	void AddHealth(UArtAttributeComponent* AttributeComp);

	/// \TODO: bad naming
	void MakeUninteractable();

	void SetVisibility_TimeElapsed();
};
