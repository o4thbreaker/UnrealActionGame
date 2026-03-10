// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArtAction.generated.h"

class UWorld;
class UArtActionComponent;

// we can rely more on struct than on two independent variables when it comes to sending data
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool IsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALACTIONGAME_API UArtAction : public UObject
{
	GENERATED_BODY()

	
public:

	void Initialize(UArtActionComponent* NewActionComponent);

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

	bool IsSupportedForNetworking() const override { return true; } 

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool IsAutoStart;

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UArtActionComponent* GetOwningComponent() const;

	UFUNCTION()
	void OnRep_RepData();

	float TimeStarted;

	UPROPERTY(Replicated)
	UArtActionComponent* ActionComponent;

	// Tags added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if OwningActor has none of these Tags applied
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;


	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	//bool IsRunning;


};
