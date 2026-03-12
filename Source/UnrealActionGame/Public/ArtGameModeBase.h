// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "ArtGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UArtSaveGame;
class UDataTable;
class UArtMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArtMonsterData* MonsterData;

	//TSubclassOf<AActor> MonsterClass;

	/* Relative chance to pick this monster*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this monster*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of coins awarded to killer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KillReward;
};

/**
 * 
 */
UCLASS()
class UNREALACTIONGAME_API AArtGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY()
	UArtSaveGame* CurrentSaveGame;

	/// <summary>
	///  \TODO: refactor to make it an array of random pickups
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TSubclassOf<AActor> PickUpItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	UEnvQuery* SpawnPickupItemsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnPickUpItems;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotsTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	float SpawnPickupItemsTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void SpawnPickUpItemsTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnSpawnItemsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	AArtGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	//BeginPlay for gamemodes
	virtual void StartPlay() override;
	
	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};
