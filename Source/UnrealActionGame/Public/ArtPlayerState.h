// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArtPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsValueChanged, int32, CoinsValue);

class UArtSaveGame;

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(ReplicatedUsing="OnRep_Coins", BlueprintReadOnly, Category = "Credits")
	int32 CurrentCoinsAmount;

	/*UFUNCTION(NetMulticast, Reliable)
	void  MulticastCoinsValueChanged();*/

	/// \NOTE: OnRep is better in this case than Multicast
	///		   because with Multicast we have to send some data (due to RPC call)
	///		   but in this case it is cheaper because of OnRep
	///		   
	///		   In AttributeComp we cant do the same trick because we have to pass
	///		   the info about the Instigator

	UFUNCTION()
	void OnRep_Coins(int32 OldCoinsAmount);

public:
	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCoinsValueChanged OnCoinsValueChanged;

public:
	AArtPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCoinsAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void SetCoinsAmount(int32 NewAmount);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UArtSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UArtSaveGame* SaveObject);
};
