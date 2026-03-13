// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArtActionEffect.h"
#include "ArtActionEffect_Thorns.generated.h"

class UArtAttributeComponent;

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API UArtActionEffect_Thorns : public UArtActionEffect
{
	GENERATED_BODY()

	/// \BUG: for some reason thorns heal the player if added to minion
	///		  i haven't diagnosed the reasons yet (but future me, u can do it)
	///		  you are the only hope, obi-wan kenobi

public:

	UArtActionEffect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effect|Thorns")
	float DamagePercentage;

	UFUNCTION(BlueprintCallable, Category = "Effect|Thorns")
	void DamageBack(AActor* DamageCauser, UArtAttributeComponent* OwningComponent, float NewHealth, float Delta);
};
