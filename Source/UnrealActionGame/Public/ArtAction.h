// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArtAction.generated.h"

class UWorld;
class UArtActionComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALACTIONGAME_API UArtAction : public UObject
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName; // fname is hashed which is pretty cool

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetIsRunning() const;

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	UArtActionComponent* GetOwningComponent() const;

	// Tags added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if OwningActor has none of these Tags applied
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool IsRunning;
};
