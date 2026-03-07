// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtAction_ProjectileAttack.h"
#include "ArtAction_BlackholeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API UArtAction_BlackholeAttack : public UArtAction_ProjectileAttack
{
	GENERATED_BODY()

public:
	virtual bool CanStart_Implementation(AActor* Instigator) override;

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float BlackholeRageCost = 30.0f;
};
