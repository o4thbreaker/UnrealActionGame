// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ArtAICharacter.h"
#include "ArtAttributeComponent.h"
#include "EngineUtils.h"
#include "ArtCharacter.h"
#include "ArtPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ArtSaveGame.h"
#include "GameFramework/GameState.h"
#include "ArtGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "ArtMonsterData.h"
#include "../UnrealActionGame.h"
#include "ArtActionComponent.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("art.SpawnBots"), true, TEXT("Enable spawning bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnItems(TEXT("art.SpawnItems"), true, TEXT("Enable spawning pick up items via timer"), ECVF_Cheat);

/// \TODO: add EQS to spawn random items

AArtGameModeBase::AArtGameModeBase()
{
	SpawnBotsTimerInterval = 2.0f;
	SpawnPickupItemsTimerInterval = 60.0f;

	SlotName = "SaveGame01";
}

void AArtGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AArtGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AArtPlayerState* PlayerState = NewPlayer->GetPlayerState<AArtPlayerState>();

	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	// start the func after adding the Player State
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AArtGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AArtGameModeBase::SpawnBotTimerElapsed, SpawnBotsTimerInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPickUpItems, this, &AArtGameModeBase::SpawnPickUpItemsTimerElapsed, SpawnPickupItemsTimerInterval, true);
}

void AArtGameModeBase::KillAll()
{
	for (TActorIterator<AArtAICharacter> It(GetWorld()); It; ++It)
	{
		AArtAICharacter* Bot = *It;

		UArtAttributeComponent* AttributeComponent = UArtAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			/// \TODO: can pass in player for kill credit
			AttributeComponent->Kill(this);
		}
	}
}

void AArtGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumberOfAliveBots = 0;

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}

	// TActorIterator is just a better (performance-wise) version of GetAllActorsOfClass
	for (TActorIterator<AArtAICharacter> It(GetWorld()); It; ++It)
	{
		AArtAICharacter* Bot = *It;

		UArtAttributeComponent* AttributeComp = UArtAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumberOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Maximum capacity of bots spawning"));
		return;
	}

	// dont know why we're passubg this as querier and not the players
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AArtGameModeBase::OnSpawnBotQueryCompleted);

	}
}

void AArtGameModeBase::SpawnPickUpItemsTimerElapsed()
{
	if (!CVarSpawnItems.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Items spawning disabled via cvar 'CVarSpawnItems'"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickupItemsQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		/// \TODO: add pathexist in eqs bp and preferable make querier as a player start (not as game mode as now)
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AArtGameModeBase::OnSpawnItemsQueryCompleted);
	}
}

void AArtGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();

			if (Manager)
			{
				// all of them
				TArray<FName> Bundles;

				// like in timers, delegate needs to be passed to bind loading to function
				// when the function gets triggered delegate will pass the params (SelectedRow->MonsterId, Locations[0])
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AArtGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);

				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}

			// so basically when we run this function we make the delegate to actually run the other function
			// the other function (that is bind to the delegate) is responsible for the actual spawn
			
			/// \NOTE: this function needed to invoke the soft reference to actual spawn

			// SUMMARY: 
			// 1. we grab random row (SelectedRow)
			// 2. we access the AssetManager (that stores the "pointers" to some assets)
			// 3. we make up the delegate and pass the necessary info
			// once LoadPrimaryAsset gets called it will send a request (while game itslef keeps running)
			// some frames later OnMonsterLoaded will be called
		}
	}
}

void AArtGameModeBase::OnSpawnItemsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn items EQS Query failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(PickUpItemClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AArtGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();

	if (Manager)
	{
		UArtMonsterData* MonsterData = Cast<UArtMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);

			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// give the guy his actions
				UArtActionComponent* ActionComponent = Cast<UArtActionComponent>(NewBot->GetComponentByClass(UArtActionComponent::StaticClass()));
				if (ActionComponent)
				{
					for (TSubclassOf<UArtAction> ActionClass : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
}

void AArtGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AArtGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AArtCharacter* Player = Cast<AArtCharacter>(VictimActor);
	AArtAICharacter* Enemy = Cast<AArtAICharacter>(VictimActor);

	if (Player)
	{
		// local because mp mode will be added
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

	if (Enemy)
	{
		// gain 3 coins
		
		/// \BUG: when KillAll called exception will be thrown
		AArtPlayerState* PlayerState = Cast<AArtPlayerState>(Cast<AArtCharacter>(Killer)->GetPlayerState());


		if (PlayerState)
		{
			/// \TODO: transfer to tweakable variable
			int32 RewardAmount = 3;
			PlayerState->SetCoinsAmount(PlayerState->GetCoinsAmount() + RewardAmount);

			PlayerState->OnCoinsValueChanged.Broadcast(PlayerState->GetCoinsAmount());
		}
		
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}


void AArtGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AArtPlayerState* PlayerState = Cast<AArtPlayerState>(GameState->PlayerArray[i]);

		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // single player only. if mp - delete this string
		}
	}
	
	// clear the array to not end up saving the same actors
	CurrentSaveGame->SavedActors.Empty();

	// iterate all the actors in the world
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (!Actor->Implements<UArtGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// pass the array to fill with data from actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		// find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;

		// convert Actor's SaveGame properties into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);

		UE_LOG(LogTemp, Log, TEXT("Actor saved: %s"), *Actor->GetName());
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AArtGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame =  Cast<UArtSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!Actor->GetClass()->ImplementsInterface(UArtGameplayInterface::StaticClass()))
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

					// find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;

					// convert Actor's SaveGame binary array back to variables
					Actor->Serialize(Ar);

					IArtGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UArtSaveGame>(UGameplayStatics::CreateSaveGameObject(UArtSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."));
	}
}

